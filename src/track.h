#include <vector>
#include "opencv2/core.hpp"
#include "Eigen/Dense"

#ifndef CPPND_CAPSTONE_DEEPSORT_TRACK_H
#define CPPND_CAPSTONE_DEEPSORT_TRACK_H

enum TrackState { Tentative, Confirmed, Deleted };


typedef Eigen::Matrix<float, 8, 1> MeanTrackValue;
typedef Eigen::Matrix<float, 8, 8> CovarianceTrackValue;
/*
 enum TrackValueIndex
 Cx, Cy: box center
 A: box aspect ratio = width / height
 H: box height
*/
enum TrackValueIndex { Cx, Cy, A, H, dCx, dCy, dA, dH};

struct TrackValue{
    MeanTrackValue mean;
    CovarianceTrackValue covariance;
};


using namespace std;
class Track {
private:
    cv::Rect box_;
    TrackValue track_value_;
    int trackId_, max_age_, n_init_;
    int hits_=1, age_=1, time_since_update_=0;
    TrackState state_ = Tentative;
    void calc_box();
public:
    Track(TrackValue &_track_value, int _trackId, int _n_init, int _max_age);
    MeanTrackValue &mean();
    CovarianceTrackValue &covariance();

    // predict tracked values using Kalman filter
    // void predict();
};

#endif //CPPND_CAPSTONE_DEEPSORT_TRACK_H
