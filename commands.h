

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

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

// you may add here helper classes

class StandardIO : public DefaultIO
{
    ifstream input;
    ofstream output;

public:
    StandardIO(string inputFile, string outputFile)
    {
        input.open(inputFile);
        output.open(outputFile);
    }

    virtual void write(string str)
    {
        cout << str << endl;
        output << str;
    }

    virtual void write(float num)
    {
        cout << num << endl;
        output << num;
    }
    virtual string read()
    {
        string str;
        input >> str;
        return str;
    }
    virtual void read(float *num)
    {
        input >> *num;
    }
    void close()
    {
        if (input.is_open())
            input.close();
        if (output.is_open())
            output.close();
    }

    ~StandardIO()
    {
        close();
    }
};

class socketIo : public DefaultIO{
    int fd;
public:
    socketIo(int fd){
        this->fd = fd;
    }
    virtual void write(string str){
        ::write(fd, str.c_str(), str.length());
    }
    virtual void write(float num){
        ::write(fd, &num, sizeof(num));
    }
    virtual string read(){
        string input = "";
        char c = 0;
        while (c != '\n'){
            input += c;
            ::read(fd, &c, sizeof(c));
        }
        return input;
    }
    virtual  void read(float *num){
        string input = "";
        char c = 0;
        while (c != '\n') {
            input += c;
            ::read(fd, &c, sizeof(c));
        }
        *num = stof(input);
    }
};
class cliSet
{
public:
    cliSet()
    {
        this->csvTest = nullptr;
        this->csvTrain = nullptr;
        HybridAnomalyDetector *hybDetector = new HybridAnomalyDetector();
        this->detector = hybDetector;
    }
    TimeSeries *csvTrain;
    TimeSeries *csvTest;
    HybridAnomalyDetector *detector;
    void setCsvTrain(const char* name){
        TimeSeries *ts = new TimeSeries(name);
        csvTrain = ts;
    }
    void setCsvTest(const char* name){
        TimeSeries *ts = new TimeSeries(name);
        csvTest = ts;
    }
    void setDetector(float threashold){
        this->detector->setThreashold(threashold);
    }
    ~cliSet()
    {
        delete csvTest;
        delete csvTrain;
        delete detector;
    }
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
    string name;
    cliSet* cli;
public:
	Command(DefaultIO* dio,cliSet *cli) : dio(dio), cli(cli){}
    Command() {}
    virtual string getName(){
        return name;
    }
	virtual void execute()=0;
	virtual ~Command(){}
};

// implement here your command classes

class Upload : public Command
{
public:
    Upload(DefaultIO* dio,cliSet* cliSet) : Command(dio, cliSet){
        Command::name = "1. Upload a time series CSV file\n";
    }
    string read(){
        string str1, str2;
        while(str1 != "done"){
            str1 = dio->read();
            str2 += str1 + "\n";
        }
        return str2;
    }
    void execute(){
        ofstream file1;
        ofstream file2;
        file1.open("Train.csv");
        file2.open("Test.csv");
        string trainFile;
        string testFile;
        dio->write("Please upload your local train CSV file.\n");
        trainFile = read();
        file1 << trainFile;
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        testFile = read();
        file2 << testFile;
        dio->write("Upload complete.\n");
        file1.close();
        file2.close();
        this->cli->setCsvTrain("Train.csv");
        this->cli->setCsvTest("Test.csv");
    }
};

class Settings : public Command{
public:
    Settings(DefaultIO *dio,cliSet *cliSet) : Command(dio, cliSet)
    {
        Command::name = "2.algorithm settings\n";
    }
    void execute(){
        float f = 7;
        float* newThreashold = &f;
        dio->write("The current correlation threshold is 0.9\n");
        dio->read(newThreashold);
        while (*newThreashold < 0 || *newThreashold > 1){
            dio->write("Please choose a value between 0 and 1.\n");
            dio->write("Please insert new threashold.");
            dio->read(newThreashold);
        }
        this->cli->setDetector(*newThreashold);
    }
};

class Detect : public Command{
public:
    Detect(DefaultIO *dio, cliSet *cliSet) : Command(dio, cliSet)
    {
        Command::name = "3. Detect anomalies.\n";
    }
    void execute(){
        this->cli->detector->learnNormal(*this->cli->csvTrain);
        dio->write("anomaly detection complete.\\n");
    }
};
class Display : public Command{
public:
    Display(DefaultIO *dio,cliSet *cliSet) : Command(dio,cliSet){
        Command::name = "4. Display results\n";
    }
    void execute(){
        vector<AnomalyReport> reports = this->cli->detector->detect(*this->cli->csvTest);
        for(AnomalyReport ar : reports){
            dio->write(to_string(ar.timeStep) + "\t" + ar.description + "\n");
        }
        dio->write("Done.\n");
    }
};

class Analyze : public Command{
public:
    Analyze(DefaultIO *dio,cliSet *cliSet) : Command(dio, cliSet)
    {
        Command::name = "5.upload anomalies and analyze results\n";
    }
    void execute(){
        dio->write("Please upload your local anomalies file.\n");
        map<string, vector<pair<long, long>>> anomalies = uniteAnomalies();
        dio->write("Upload complete.\n");
        long positive = 0;
        long num1, num2;
        string str = dio->read();
        vector<pair<long, long>> timeSteps;
        while (str != "done"){
            positive++;
            stringstream line(str);
            string num;
            getline(line, num,',');
            num1 = stol(num);
            getline(line, num, ',');
            num2 = stol(num);
            pair<long, long> p;
            p.first = num1;
            p.second = num2;
            timeSteps.push_back(p);
            str = dio->read();
        }
        long negative = this->cli->csvTest->result[0].second.size();
        long reportsSum;
        for(int i = 0; i < timeSteps.size(); i++){
            reportsSum += timeSteps.at(i).second - timeSteps.at(i).first + 1;
        }
        negative -= reportsSum;
        double TP = 0, FP = 0;
        long FPCount = 0;
        for(map<string, vector<pair<long, long>>>::iterator it = anomalies.begin(); it != anomalies.end(); ++it){
            for(int i = 0;i < it->second.size(); i++){
                FP++;
                for(int j = 0; j < timeSteps.size(); j++){
                    if(!(it->second.at(i).first > timeSteps.at(j).second || timeSteps.at(j).first > it->second.at(i).second)){
                        TP++;
                        FPCount++;
                    }
                }
            }
        }
        double TPR = TP / positive;
        TPR = floor(TPR * 1000);
        TPR = TPR / 1000;
        FP -= FPCount;
        double AFR = FP / negative;
        AFR = floor(AFR * 1000);
        AFR = AFR / 1000;
        stringstream ss;
        ss << TPR;
        string str1 = ss.str();
        this->dio->write("True Positive Rate: " + str1 + '\n');
        stringstream ss1;
        ss1 << AFR;
        string str2 = ss1.str();
        this->dio->write("False Positive Rate: " + str2 + '\n');
    }
    map<string, vector<pair<long,long>>> uniteAnomalies(){
        map<string, vector<pair<long, long>>> anomalies;
        vector<pair<long, long>> timeSteps;
        vector<AnomalyReport> reports = this->cli->detector->detect(*cli->csvTest);
        long index1, index2, final;
        string report1, report2;
        index1 = reports.at(0).timeStep;
        report1 = reports.at(0).description;
        final = index1;
        for(int i = 0; i < reports.size(); i++){
            if(i != reports.size() - 1){
                index2 = reports.at(i + 1).timeStep;
                report2 = reports.at(i + 1).description;
            }
            if(report1 == report2 && (final + 1 == index2)){
                final++;
                continue;
            }
            else
            {
                pair<long, long> pair(index1, final);
                timeSteps.push_back(pair);
                anomalies.insert({report1, timeSteps});
                report1 = report2;
                index1 = index2;
                final = index1;
                timeSteps.clear();
            }
        }
        return anomalies;
    }
};

class Exit : public Command{
public:
    Exit(DefaultIO *dio, class cliSet *cliSet) : Command(dio, cliSet){
        Command::name = "6.exit\n";
    }
    void execute()
    {
        return;
    }
};

#endif /* COMMANDS_H_ */
