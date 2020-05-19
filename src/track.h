#include <vector>
#include "opencv2/core.hpp"
#include "KalmanFilter.h"
#include "detection.h"

#ifndef CPPND_CAPSTONE_DEEPSORT_TRACK_H
#define CPPND_CAPSTONE_DEEPSORT_TRACK_H


enum TrackState { Tentative, Confirmed, Deleted };
using namespace std;
class Track {
private:
    int trackId_;                   // A unique track identifier.

    int max_age_;                   // The maximum number of consecutive misses before the track state
                                    // is set to `Deleted`.

    int hits_=1;                    // Total number of measurement updates.
    int age_=1;                     // Total number of frames since first occurance.
    int time_since_update_=0;       // Total number of frames since last measurement update

    int n_init_;                    // Number of consecutive detections before the track is confirmed.
                                    // The track state is set to `Deleted` if a miss occurs within the
                                    // first `n_init` frames

    TrackState state_ = Tentative;
    TrackValue track_value_;

public:
    Track(TrackValue &_track_value, int _trackId, int _n_init, int _max_age);
    void predict(KalmanFilter &kf);
    void update(KalmanFilter &kf, Detection &det);
    void mark_missed();

    // check state
    bool is_tentative(){return state_ == Tentative;}
    bool is_confirmed(){return state_ == Confirmed;}
    bool is_deleted(){return state_ == Deleted;}

    // getter
    vector<double> to_tlbr();
    cv::Rect box();
    TrackState state(){return state_;}
    int trackID (){return trackId_;}
    TrackValue track_value(){return track_value_;}

    MatchState mstate = Unmatched;
};

#endif //CPPND_CAPSTONE_DEEPSORT_TRACK_H