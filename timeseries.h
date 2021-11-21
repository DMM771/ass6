

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include "iostream"
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

class TimeSeries{
public:
    vector<pair<string,vector<float>>> result;
    map<string,vector<float>> mp;
public:
    TimeSeries(const char* CSVfileName);
};
#endif /* TIMESERIES_H_ */
