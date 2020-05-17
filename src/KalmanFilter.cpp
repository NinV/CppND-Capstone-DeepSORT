#include "iostream"
#include "cmath"
#include "KalmanFilter.h"


KalmanFilter::KalmanFilter(){
    motion_mat_ = Eigen::Matrix<double , 8, 8>::Identity();

    // motion_Cx = Cx + dCx
    motion_mat_(Cx, dCx) = 1;

    // motion_Cy = Cy + dCy
    motion_mat_(Cy, dCy) = 1;

    // motion_A = A + dA
    motion_mat_(A, dA) = 1;

    // motion_H = H + dH
    motion_mat_(H, dH) = 1;

    update_mat_ = Eigen::Matrix<double, 4, 8>::Identity();
};


TrackValue KalmanFilter::initiate(const Measurement &m) {
    StateVector state = StateVector::Zero();
    StateCovariance covariance = StateCovariance::Zero();

    state.block<4, 1>(0,0) = m;

    double position_covariance = 2 * std_weight_position_ * m(H);
    double velocity_covariance = 10 * std_weight_velocity_ * m(H);

    // diagonal matrix
    covariance(Cx, Cx) = position_covariance;
    covariance(Cy, Cy) = position_covariance;
    covariance(A, A) = 1e-2;
    covariance(H, H) = position_covariance;
    covariance(dCx, dCx) = velocity_covariance;
    covariance(dCy, dCy) = velocity_covariance;
    covariance(dA, dA) = 1e-5;
    covariance(dH,dH) = velocity_covariance;
    covariance *= covariance.transpose();

    return TrackValue(state, covariance);
}


void KalmanFilter::predict(TrackValue &t){

    StateCovariance motion_cov = StateCovariance::Zero();
    double position_covariance = std_weight_position_ * t.state_vector(H);
    double velocity_covariance = std_weight_velocity_ * t.state_vector(H);
    motion_cov(Cx, Cx) =  position_covariance;
    motion_cov(Cy, Cy) =  position_covariance;
    motion_cov(A, A) =  1e-2;
    motion_cov(H, H) =  position_covariance;
    motion_cov(dCx, dCx) =  velocity_covariance;
    motion_cov(dCy, dCy) =  velocity_covariance;
    motion_cov(dA, dA) =  1e-5;
    motion_cov(dH, dH) =  velocity_covariance;
    motion_cov *= motion_cov.transpose();

    t.state_vector = motion_mat_ * t.state_vector;
    t.covariance = motion_mat_ * t.covariance * motion_mat_.transpose() + motion_cov;

}


ProjectedValue KalmanFilter::project(const TrackValue &t) {
    Eigen::Matrix<double, 4, 4> innovation_cov = Eigen::Matrix<double, 4, 4>::Zero();
    double position_covariance = pow(std_weight_position_ * t.state_vector(H), 2);

    innovation_cov(Cx, Cx) = position_covariance;
    innovation_cov(Cy, Cy) = position_covariance;
    innovation_cov(A, A) = 1e-2;
    innovation_cov(H, H) = position_covariance;

    std::cout << "innovation_cov:\n" << innovation_cov << "\n";

    ProjectedValue projected;
    projected.p_state = update_mat_ * t.state_vector;
    projected.p_covariance = update_mat_ * t.covariance * update_mat_.transpose() + innovation_cov;

    std::cout << "p_state:\n" << projected.p_state << "\n";
    std::cout << "p_covariance:\n" << projected.p_covariance << "\n";

    return projected;
}


void KalmanFilter::update(TrackValue &t, Measurement &m) {
    ProjectedValue projected = project(t);
    auto kalman_gain = t.covariance * update_mat_.transpose() * projected.p_covariance.inverse();
    auto innovation = m - projected.p_state;

    std::cout << "kalman_gain:\n" << kalman_gain << "\n";
    std::cout << "innovation:\n" << innovation << "\n";

    t.state_vector = t.state_vector + (kalman_gain * innovation);
    std::cout << "updated state_vector:\n" << t.state_vector << "\n";
    t.covariance = t.covariance - kalman_gain * projected.p_covariance * kalman_gain.transpose();
    std::cout << "updated covariance:\n" << t.covariance << "\n";
}