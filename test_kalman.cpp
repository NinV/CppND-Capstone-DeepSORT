//
// Created by ninv on 5/16/20.
//
#include "src/KalmanFilter.h"
#include <iostream>

int main(){
    KalmanFilter kf = KalmanFilter();
    Measurement v;
    v << 300, 250, 1.5, 50;
    TrackValue value = kf.initiate(v);


    std::cout << "Initiate:\n" << value.state_vector.transpose() << "\n\n" << value.covariance << "\n";

    value.state_vector.block<4, 1>(4, 0) << 3, 5, 0.1, 1;

    std::cout << "Add motion:\n" << value.state_vector.transpose() << "\n";

    kf.predict(value);
    std::cout << "Predict:\n" << value.state_vector.transpose() << "\n\n" << value.covariance << "\n";

//    kf.project(value);

    Measurement nv;
    nv << 307, 249, 1.556, 506;
    kf.update(value, nv);
}
