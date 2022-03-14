//324680438
//313306367
// Nitzan Fisher & David Monheit

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;


class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string someString) = 0;

    virtual void write(float x) = 0;

    virtual void read(float *x) = 0;

    virtual ~DefaultIO() {}

    void scanAndSeekF(string name) {
        ofstream out(name);
        string s = "";
        s = read();
        while ((s != "done\n") && (s != "done") && (strcmp(s.c_str(), "done") != 0) &&
                (strcmp(s.c_str(), "done\n") != 0)) {
            out << s << endl;
            s = read();
        }
        out.close();
    }
    // you may add additional methods here
};

// you may add here helper classes

class StandardIO : public DefaultIO {
    ifstream inStream;
    ofstream outStream;

public:
    StandardIO(string inputFile, string outputFile) {
        inStream.open(inputFile);
        outStream.open(outputFile);
    }

    virtual void write(string str) {
        cout << str << endl;
        outStream << str;
    }

    virtual void write(float num) = 0;

    virtual string read() {
        string str;
        inStream >> str;
        return str;
    }

    virtual void read(float *num) {
        inStream >> *num;
    }

    void close() {
        if (inStream.is_open())
            inStream.close();
        if (outStream.is_open())
            outStream.close();
    }

    ~StandardIO() {
        close();
    }
};


struct specialReport {
    int initTime;
    int finalTime;
    bool trueP;
    string desc;
};

struct State {
    float thresh;
    vector<specialReport> specialReps;
    int fileSize;
    vector<AnomalyReport> reps;

    State() {
        fileSize = 0;

        thresh = 0.9;

    }
};

// you may edit this class
class Command {
protected:
    DefaultIO *defIo;
public:
    const string desc;

    Command(DefaultIO *dio, const string desc) : defIo(dio), desc(desc) {}

    Command() {}

    virtual void execute(State *state) = 0;

    virtual ~Command() {}
};

// implement here your command classes

class UploadCmd : public Command {
public:
    UploadCmd(DefaultIO *defIo) : Command(defIo,
                                          "upload a time series csv file") {
    }

    virtual void execute(State *state) {
        defIo->write("Please upload your local train CSV file.\n");
        defIo->scanAndSeekF("anomalyTrain.csv");

        defIo->write("UploadCmd complete.\n");

        defIo->write("Please upload your local test CSV file.\n");
        // then scan again
        defIo->scanAndSeekF("anomalyTest.csv");
        defIo->write("UploadCmd complete.\n");
    }
};

class SettingsCmd : public Command {
public:
    SettingsCmd(DefaultIO *dio) : Command(dio, "algorithm settings") {
    }

    virtual void execute(State *state) {
        bool isAlright = false;
        while (!isAlright) {
            defIo->write("The current correlation threshold is ");
            defIo->write(state->thresh);
            defIo->write("\nType a new threshold\n");
            float x;
            defIo->read(&x);
            if (x > 0 && x <= 1) {
                state->thresh = x;
                isAlright = true;
            } else
                defIo->write("please choose a value between 0 and 1.\n");
        }
    }
};

class DetectCmd : public Command {
public:
    DetectCmd(DefaultIO *dio) : Command(dio, "detect anomalies") {
    }

    virtual void execute(State *state) {
        TimeSeries train("anomalyTrain.csv");
        TimeSeries test("anomalyTest.csv");
        state->fileSize = test.getWidth();
        HybridAnomalyDetector haDetector;
        // setting the threshold
        haDetector.setThreashold(state->thresh);
        haDetector.learnNormal(train);
        state->reps = haDetector.detect(test);

        specialReport rep;
        rep.initTime = 0;
        rep.finalTime = 0;
        rep.desc = "";
        rep.trueP = false;
        for_each(state->reps.begin(), state->reps.end(), 
                 [&rep, state](AnomalyReport &repo) {
            if (repo.timeStep == rep.finalTime + 1) {
                if(repo.description == rep.desc)
                    rep.finalTime++;
            }
            else {
                state->specialReps.push_back(rep);

                rep.initTime = repo.timeStep;

                rep.finalTime = rep.initTime;
                rep.desc = repo.description;
            }
        });
        state->specialReps.push_back(rep);
        state->specialReps.erase(state->specialReps.begin());

        defIo->write("anomaly detection complete.\n");
    }
};

class DisplayCmd : public Command {
public:
    DisplayCmd(DefaultIO *dio) : Command(dio, "display results") {
    }

    virtual void execute(State *state) {
        for (AnomalyReport rep: state->reps) {
            defIo->write(to_string(rep.timeStep) + "\t" + rep.description + "\n");
        }
        defIo->write("Done.\n");
    }
};

class Analyze : public Command {
public:
    Analyze(DefaultIO *dio) : Command(dio, "upload anomalies and analyze results") {
    }

    bool isTruePositive(int startTime, int endTime, State *sharedState) {
        for (size_t i = 0; i < sharedState->specialReps.size(); i++) {
            specialReport rep = sharedState->specialReps[i];
            if (endTime >= rep.initTime && rep.finalTime >= startTime) {
                sharedState->specialReps[i].trueP = true;
                return true;
            }
        }
        return false;
    }

    virtual void execute(State *state) {
        for (int i = 0; i < state->specialReps.size(); i++) {
            state->specialReps[i].trueP = false;
        }

        defIo->write("Please upload your local anomalies file.\n");
        string str = "";
        float trueP = 0;
        float total = 0;
        float positiveCount = 0;
        str = defIo->read();
        while ((str != "done\n") && (str != "done") && (strcmp(str.c_str(), "done") != 0) &&
               (strcmp(str.c_str(), "done\n") != 0)) {
            int counter = 0;
            for (int d = 0; str[counter] != ','; counter++);
            string starter = str.substr(0, counter);
            string ender = str.substr(counter + 1, str.length());
            int startIndex = stoi(starter);
            int endIndex = stoi(ender);
            if (isTruePositive(startIndex, endIndex, state))
                trueP++;
            total += endIndex + 1 - startIndex;
            positiveCount++;
            str = defIo->read();
        }
        defIo->write("Upload complete.\n");
        float FalsePos = 0;
        for (size_t i = 0; i < state->specialReps.size(); i++)
            if (!state->specialReps[i].trueP)
                FalsePos++;

        float Number = state->fileSize - total;
        float truePosR = ((int) (1000.0 * trueP 
                / positiveCount)) / 1000.0f;
        float falsePosR = ((int) (1000.0 * FalsePos 
                / Number)) / 1000.0f;
        defIo->write("True Positive Rate: ");
        defIo->write(truePosR);
        defIo->write("\nFalse Positive Rate: ");
        defIo->write(falsePosR);
        defIo->write("\n");
    }
};

class ExitCmd : public Command {
public:
    ExitCmd(DefaultIO *dio) : Command(dio, "exit") {
    }

    virtual void execute(State *state) {
        return;
    }
};

#endif /* COMMANDS_H_ */
