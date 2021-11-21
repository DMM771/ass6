#include "timeseries.h"

TimeSeries::TimeSeries(const char* CSVfileName){
    //open stream for file
    ifstream infoFile(CSVfileName);
    //set row, colName and val for use in iteration
    string row, colName;
    float val;
    //check that file has opened correctly
    if(infoFile.good()){
        //create string stream of line
        getline(infoFile, row);
        stringstream ss(row);
        //loop to get information seperated by commas
        while(getline(ss, colName, ',')){
            // set <colName, float vector> pairs to result
            result.push_back({colName,vector<float> {}});
        }
    }
    // loop to fill remaining rows with information
    while (getline(infoFile, row)){
        stringstream ss(row);
        //start at first column
        int column = 0;
        //loop to store each value in val
        while (ss >> val){
            //finally, add to second pair of vector
            result.at(column).second.push_back(val);
            //ignore ','
            if(ss.peek() == ',') ss.ignore();
            column++;
        }
    }

    for (int i = 0; i < result.size(); i++) {
        cout << "column name is: " << result[i].first;
        cout << endl;
        for (int j = 0; j < result[i].second.size(); j++) {
            cout << result[i].second[j] << " ";
        }
        cout << endl << endl << endl;
    }
    infoFile.close();
}