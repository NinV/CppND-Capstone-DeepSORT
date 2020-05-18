#include <vector>
#include <set>
#include <iomanip>
#include "track.h"
#include "detection.h"
#include "KalmanFilter.h"
#include "iou_matching.h"
#include "hungarian.h"

#ifndef CPPND_CAPSTONE_DEEPSORT_TRACKER_H
#define CPPND_CAPSTONE_DEEPSORT_TRACKER_H


using namespace std;
struct MatchResult{
    vector<Detection> matched_detections;
    vector<Detection> unmatched_detections;
    vector<Track> matched_tracks;
    vector<Track> unmatched_tracks;

    void add_matched(Track &t, Detection &det){
        matched_tracks.push_back(t);
        matched_detections.push_back(det);
    };
};


class Tracker {
private:
    vector<Track> tracks_;
    KalmanFilter kf_ = KalmanFilter();
    int max_age;
    int n_init;
    double max_iou_distance;
    int total_tracks=0;

    MatchResult match(vector<Detection> &detections);
    int next_track_idx();

public:
    explicit Tracker(double _max_iou_distance=0.7, int _max_age = 30, int _n_init = 3):
                                    max_iou_distance(_max_iou_distance),
                                    max_age(_max_age),
                                    n_init(_n_init){};
    void predict();
    void update(vector<Detection> &detections);
    vector<Track> &tracks(){ return tracks_;}

};


#endif //CPPND_CAPSTONE_DEEPSORT_TRACKER_H
