#include <math.h>
#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(){
    motion_mat_ = Eigen::MatrixXd::Identity(8, 8);

    // motion_Cx = Cx + dCx
    motion_mat_(Cx, dCx) = 1;

    // motion_Cy = Cy + dCy
    motion_mat_(Cy, dCy) = 1;

    // motion_A = A + dA
    motion_mat_(A, dA) = 1;

    // motion_H = H + dH
    motion_mat_(H, dH) = 1;
};


TrackValue KalmanFilter::initiate(Detection &detection) {
    MeanTrackValue mean = MeanTrackValue::Zero();
    CovarianceTrackValue covariance = CovarianceTrackValue::Zero();

    mean(Cx) = detection.x + detection.w / 2;
    mean(Cy) = detection.y + detection.h / 2;
    mean(A) = detection.w / detection.h;
    mean(H) = detection.h;

    float position_covariance = pow(2 * std_weight_position_ * detection.h, 2);
    float velocity_covariance = pow(10 * std_weight_velocity_ * detection.h, 2);
    // diagonal matrix
    covariance(Cx, Cx) = position_covariance;
    covariance(Cy, Cy) = position_covariance;
    covariance(A, A) = 1e-2;
    covariance(dCx, dCx) = velocity_covariance;
    covariance(dCy, dCy) = velocity_covariance;
    covariance(dA, dA) = 1e-5;
    covariance(dH,dH) = velocity_covariance;

    return TrackValue(mean, covariance);
}
