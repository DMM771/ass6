//324680438
//313306367
// Nitzan Fisher & David Monheit

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <map>
using namespace std;

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Circle minCircle;
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

    int getLinesNum(const TimeSeries &ts);

    virtual void makeReport(map<string, vector<float>> map, vector<correlatedFeatures> cf, vector<AnomalyReport> &vec,
               long timeStep);

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
