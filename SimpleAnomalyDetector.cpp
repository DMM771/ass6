
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

void **getPoints(Point **points, float *a, float *b, int size){
    for (int i =0; i < size; i ++){
        points[i] = new Point(a[i], b[i]);
    }
}
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    string feature1,feature2;
    Point ** points;
    for(int i = 0; i < ts.result.size(); i++){
        feature1 = ts.result[i].first;
        float m = 0;
        int c = -1;
        for(int j = i + 1; j < ts.result.size(); j++){
            float const *a = ts.result[i].second.data();
            auto * aNonConst = const_cast<float *>(a);
            float const *b = ts.result[j].second.data();
            auto * bNonConst = const_cast<float *>(b);
            int size = min(ts.result[i].second.size(), ts.result[j].second.size());
            //get absolute value from pearson
            float p = abs(pearson(aNonConst, bNonConst, size));
            if(p > m){
                //set local feature2 to correct name
                feature2 = ts.result[j].first;
                m = p;
                c = j;
            }
            // if c != -1, correlation has been found, associate
            if (c != -1){
                correlatedFeatures cor = correlatedFeatures();
                points = new Point*[size];
                getPoints(points, aNonConst, bNonConst, size);
                //create new correlation and update fields
                //cor = new correlatedFeatures;
                cor.feature1 = feature1;
                cor.feature2 = feature2;
                cor.lin_reg = linear_reg(points, size);
                cor.correlation = m;
                cf.push_back(cor);
                float maxDev = 0;
                for(int l = 0; l < size; l++){
                    float temDev = dev((*points)[l], cor.lin_reg);
                    if (temDev > maxDev){
                        maxDev = temDev;
                    }
                }
                cor.threshold = (float) 1.1 * maxDev ;
            }
        }
    }
}

int SimpleAnomalyDetector::getLinesNum(const TimeSeries& ts){
    int m = 0;
    int temp = 0;
    for(int i = 0; i < ts.result.size(); i++){
        for(int j = 0; j < ts.result.at(i).second.size(); j++){
            temp++;
        }
            if(temp > m){
                m = temp;
            }
            temp = 0;
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> reportVec;
    float val1, val2;
    for(int i = 0; i < getLinesNum(ts); ++i){
        for(int j = 0; j < cf.size(); ++j){
            val1 = getValue(ts, cf.at(j).feature1, i);
            val2 = getValue(ts, cf.at(j).feature2, i);
            Point *p = new Point(val1, val2);
            Point p1 = *p;
            if(dev(p1, cf.at(j).lin_reg) > cf.at(j).threshold){
                const string description = cf.at(j).feature1 + "-" + cf.at(j).feature2;
                long timeStep = i + 1;
                AnomalyReport ar(description, timeStep);
                reportVec.push_back(ar);
            }
            delete p;
        }
    }
    return reportVec;
}

float SimpleAnomalyDetector::getValue(const TimeSeries& ts, string feature, int index){
    for(int i = 0; i < ts.result.size(); ++i){
        if(ts.result.at(i).first == feature){
            return ts.result.at(i).second.at(index);
        }
    }
}

