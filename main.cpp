#include <iostream>
#include "anomaly_detection_util.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    float l[] = {1,2,3,4,5,6,7,8,9};
    float p = var(l,9);
    std::cout << p << std::endl;
//change12
    return 0;
}
