//324680438
//313306367
/*************
* Nitzan Fisher & David Monheit
* ex2
* ************/
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_
#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <map>
using namespace std;
//the struct that holds the correlated features couple.
struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    //the threshold is 1.1 * the max deviation from the linear reg.
    float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    float getValue(const TimeSeries &ts, string feature, int index);
    int getLinesNum(const TimeSeries &ts);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
