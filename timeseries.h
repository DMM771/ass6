

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include "iostream"
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

class TimeSeries{
public:
    vector<pair<string,vector<float>>> result;
public:
    TimeSeries(const char* CSVfileName);
};
#endif /* TIMESERIES_H_ */
