cmake_minimum_required(VERSION 3.20)
project(ass1)

set(CMAKE_CXX_STANDARD 11)

add_executable(ass1 MainTrain.cpp anomaly_detection_util.h anomaly_detection_util.cpp AnomalyDetector.h SimpleAnomalyDetector.h SimpleAnomalyDetector.cpp timeseries.h timeseries.cpp minCircle.cpp minCircle.h HybridAnomalyDetector.cpp
        )
