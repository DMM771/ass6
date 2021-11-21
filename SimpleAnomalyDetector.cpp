//324680438
//313306367
/*************
* Nitzan Fisher & David Monheit
* ex2
* ************/
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

/******
* Function Name: getPoints
* Input: float* a, float* b, size - two arrays of floats and their size.
* Output: vector of points.
* Function Operation: the function gets two arrays of floats and returns vector of points that the x,y values
 * of every point are the values from the array.
******/
vector<Point> getPoints(float *a, float *b, int size){
    vector<Point> listOfPoints;
    listOfPoints.reserve(size);
    //makes the points.
    for (int i =0; i < size; i++){
        listOfPoints.emplace_back(a[i], b[i]);
    }
    return listOfPoints;
}
/******
* Function Name: learnNormal
* Input: const TimeSeries& ts - time series.
* Output: void.
* The function gets a time series and then check every couple of columns if there is a correlation between
 * them, if so it saves for every collumn the collumn that is most correlated to. the function fills
 * the correlated features vector cf.
******/
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    float minCorrelation = 0.9;
    float * aNonConst;
    float * bNonConst;
    float *finalConsA;
    float *finalConsB;
    int size;
    string feature1,feature2;
    vector<Point> listOfPoints;
    Point **points;
    for(int i = 0; i < ts.result.size(); i++){
        feature1 = ts.result[i].first;
        float m = 0;
        int c = -1;
        //checks for the column i the correlation with every column from i+1 to the last column.
        for(int j = i + 1; j < ts.result.size(); j++){
            float const *a = ts.result[i].second.data();
            aNonConst = const_cast<float *>(a);
            float const *b = ts.result[j].second.data();
            bNonConst = const_cast<float *>(b);
            size = min(ts.result[i].second.size(), ts.result[j].second.size());
            //get absolute value from pearson.
            float p = abs(pearson(aNonConst, bNonConst, size));
            if(p > m){
                finalConsA = aNonConst;
                finalConsB = bNonConst;
                //set local feature2 to correct name
                feature2 = ts.result[j].first;
                m = p;
                c = j;
            }
        }
        // if c != -1, correlation has been found, make a correlated features couple and fill cf.
        if (c != -1 && m >= minCorrelation){
            correlatedFeatures cor = correlatedFeatures();
            listOfPoints = getPoints(finalConsA, finalConsB, size);
            //create new correlation and update fields
            //cor = new correlatedFeatures;
            cor.feature1 = feature1;
            cor.feature2 = feature2;
            Point *p = listOfPoints.data();
            points = &p;
            cor.lin_reg = linear_reg(points, size);
            cor.corrlation = m;
            float maxDev = 0;
            //check the max deviation for calculating the threshold.
            for(int l = 0; l < size; l++){
                float temDev = dev(listOfPoints[l], cor.lin_reg);
                if (temDev > maxDev){
                    maxDev = temDev;

                }
            }
            cor.threshold = (float) 1.1 * maxDev ;
            cf.push_back(cor);
        }
    }
}
/******
* Function Name: detect
* Input: const TimeSeries& ts - time series.
* Output: void.
* The function checks for every line in the time series if there are anomalies according to the
 * correlated features that found in the previous step. the function check for every couple data (according to
 * the couple that are in the correlated features vector) if the point that they are making is too far
 * from the linear reg of the correlated feature they belong to..
******/


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> reportVec;
    string feature1, feature2;
    for(int i = 0; i < ts.result[0].second.size(); ++i){
        for (correlatedFeatures c:cf){
            feature1 = c.feature1;
            feature2 = c.feature2;
            Point p = Point(ts.mp.at(feature1).at(i),ts.mp.at(feature2).at(i));
            //checks if the point is too far.
            if(dev(p, c.lin_reg) > c.threshold){
                const string description = c.feature1 + "-" + c.feature2;
                long timeStep = i + 1;
                AnomalyReport ar(description, timeStep);
                reportVec.push_back(ar);
            }
        }
    }
    return reportVec;
}


