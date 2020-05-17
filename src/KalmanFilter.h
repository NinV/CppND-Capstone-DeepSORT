#include "Eigen/Dense"


#ifndef CPPND_CAPSTONE_DEEPSORT_KALMANFILTER_H
#define CPPND_CAPSTONE_DEEPSORT_KALMANFILTER_H

typedef Eigen::Matrix<double , 8, 1> StateVector;
typedef Eigen::Matrix<double , 8, 8> StateCovariance;
enum StateVectorIndex {
    Cx,      // box center x
    Cy,      // box center x
    A,       // aspect ratio = width / height
    H,       // box height
    dCx,
    dCy,
    dA,
    dH
};

// Measurement's elements: Cx, Cy, A and H
typedef Eigen::Matrix<double , 4, 1> Measurement;

// Holder for state vector and state covariance
struct TrackValue {
    StateVector state_vector;
    StateCovariance covariance;

    TrackValue() = default;

    TrackValue(StateVector &_state_vector,
               StateCovariance &_covariance) : state_vector(_state_vector), covariance(_covariance) {};
};


//typedef Eigen::Matrix<double , 4, 1> ProjectedStateVector;
typedef Eigen::Matrix<double , 4, 4> ProjectedCovariance;

// Holder for projected state vector and projected state covariance
struct ProjectedValue {
    Measurement p_state;
    ProjectedCovariance p_covariance;

    ProjectedValue() = default;

    ProjectedValue(Measurement &_p_state,
                   ProjectedCovariance &_p_covariance) : p_state(_p_state), p_covariance(_p_covariance) {};
};


class KalmanFilter {
private:
    constexpr static double std_weight_position_ = 1. / 20;
    constexpr static double std_weight_velocity_ = 1. / 160;
    Eigen::Matrix<double , 8, 8> motion_mat_;
    Eigen::Matrix<double, 4, 8> update_mat_;

public:
    KalmanFilter();

    //  create a TrackValue instance from an unassociated Detection.
    TrackValue initiate(const Measurement &m);

    //  run Kalman filter prediction step
    void predict(TrackValue &t);

    //  run Kalman filter correction step
    void update(TrackValue &t, Measurement &m);

    ProjectedValue project(const TrackValue &t);
};

#endif //CPPND_CAPSTONE_DEEPSORT_KALMANFILTER_H