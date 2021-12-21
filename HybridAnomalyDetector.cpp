
#include "HybridAnomalyDetector.h"
HybridAnomalyDetector::HybridAnomalyDetector() {
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
}

float distance(Point a,Point b)
{
    return sqrt(pow(a.x - b.x, 2)
                + pow(a.y - b.y, 2));
}


void HybridAnomalyDetector::makeReport(map<string, vector<float>> map, vector<correlatedFeatures> cf, vector<AnomalyReport> &vec,
                long timeStep){
    for (correlatedFeatures c:cf) {
        string feature1 = c.feature1;
        string feature2 = c.feature2;
        Point p = Point(map.at(feature1).at(timeStep),map.at(feature2).at(timeStep));
        if(c.corrlation > 0.9){
            if(dev(p, c.lin_reg) > c.threshold){
                const string description = feature1 + "-" + feature2;
                AnomalyReport ar(description, timeStep + 1);
                vec.push_back(ar);
                }
            }
        if(c.corrlation <= 0.9){
                if(distance(p, c.minCircle.center) > c.threshold){
                    const string description = feature1 + "-" + feature2;
                    AnomalyReport ar(description, timeStep + 1);
                    vec.push_back(ar);
            }
        }
    }
}

