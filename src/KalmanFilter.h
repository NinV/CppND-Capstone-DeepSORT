#include "detection.h"
#include "track.h"


#ifndef CPPND_CAPSTONE_DEEPSORT_KALMANFILTER_H
#define CPPND_CAPSTONE_DEEPSORT_KALMANFILTER_H


class KalmanFilter {
private:
    constexpr static float std_weight_position_ = 1. / 20;
    constexpr static float std_weight_velocity_ = 1. / 160;
    Eigen::Matrix<float, 8, 8> motion_mat_;
    Eigen::Matrix<float, 4, 8> update_mat_;

public:
    KalmanFilter();

    //  create a TrackValue instance from an unassociated Detection.
    TrackValue initiate(Detection &detection);

    //  run Kalman filter prediction step and modify the track object
    void predict(Track &track);

    //  run Kalman filter correction step and modify the track object.
    void update(Track &track, Detection &detection);
};


#endif //CPPND_CAPSTONE_DEEPSORT_KALMANFILTER_H