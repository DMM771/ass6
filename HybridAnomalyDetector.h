

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void makeReport(map<string, vector<float>> map, vector<correlatedFeatures> cf, vector<AnomalyReport> &vec,
                    long timeStep);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
