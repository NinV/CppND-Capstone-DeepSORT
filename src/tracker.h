#include <vector>
#include <map>
#include <memory.h>
#include <utility>
#include "track.h"
#include "detection.h"
#include "KalmanFilter.h"
#include "iou_matching.h"
#include "hungarian.h"

#ifndef CPPND_CAPSTONE_DEEPSORT_TRACKER_H
#define CPPND_CAPSTONE_DEEPSORT_TRACKER_H


using namespace std;
//struct MatchResult{
//    vector<int> matched_detections_idx;
//    vector<int> unmatched_detections_idx;
//    vector<int> matched_trackIds;
//    vector<int> unmatched_trackIds;
//
//    void add_matched(int t_idx, int det_idx){
//        matched_detections_idx.push_back(det_idx);
//        matched_trackIds.push_back(t_idx);
//    };
//
//    void add_unmatched_track(int t_idx){
//        unmatched_trackIds.push_back(t_idx);
//    }
//
//    void add_unmatched_detection(int det_idx){
//        unmatched_detections_idx.push_back(det_idx);
//    }
//};


class Tracker {
private:
    map<int, shared_ptr<Track>> tracks_;
    KalmanFilter kf_ = KalmanFilter();
    int max_age;
    int n_init;
    double max_iou_distance;
    int total_tracks=0;

    vector<pair<shared_ptr<Track>, Detection*>> match(vector<Detection> &detections);
    void reset_matched();
    int next_track_idx();

public:
    explicit Tracker(double _max_iou_distance=0.7, int _max_age = 30, int _n_init = 3):
                                    max_iou_distance(_max_iou_distance),
                                    max_age(_max_age),
                                    n_init(_n_init){};
    void initiate_track(TrackValue tv);
    void remove_track(int trackId);
    void predict();
    void update(vector<Detection> &detections);

    vector<shared_ptr<Track>> tracks();
};


#endif //CPPND_CAPSTONE_DEEPSORT_TRACKER_H
