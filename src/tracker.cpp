    #include "tracker.h"

void Tracker::predict() {
    for(Track track:tracks_){track.predict(kf_);}
}

void Tracker::update(vector <Detection> &detections) {

    MatchResult m = match(detections);

    // update tracks that have associated detection
    for(int i=0; i < m.matched_tracks.size(); i++){
        m.matched_tracks[i].update(kf_, m.matched_detections[i]);
    }

    // mark missed for tentative tracks and confirmed track that doesn't matched
    for (auto &t : m.unmatched_tracks){
        t.mark_missed();
    }

    // remove tracks are marked as Deleted
    vector<Track> tmp;
    for (auto &t : tracks_){
        if (!t.is_deleted()){
            tmp.push_back(t);
        }
    }

    tracks_ = tmp;

    for (auto &det : m.unmatched_detections){
        TrackValue tv = kf_.initiate(det.to_xyah());
        tracks_.emplace_back(Track(tv, next_track_idx(), n_init, max_age));
    }
}

int Tracker::next_track_idx() {
    total_tracks += 1;
    return total_tracks;
}

MatchResult Tracker::match(vector<Detection> &detections) {
    auto iou_track_candidates = tracks_;

    // create cost matrix for iou matching
    vector<vector<double>> t_boxes, det_boxes;
    for (Detection &det: detections){
        det_boxes.push_back(det.to_tlbr());
    }
    for (Track &t: iou_track_candidates){
        t_boxes.push_back(t.to_tlbr());
    }
    auto cost_matrix = iou_cost(t_boxes, det_boxes, max_iou_distance);

    // matching Track instances to Detection instances using Hungarian (Munkres) algorithm
    HungarianAlgorithm HungAlgo;
    vector<int> assignments;

    MatchResult mresult;
    std::set<int> confirmed_det_idxs;
    double cost = HungAlgo.Solve(cost_matrix, assignments);

    for (int track_idx = 0; track_idx < cost_matrix.size(); track_idx++){
        int det_idx = assignments[track_idx];
        if (cost_matrix[track_idx][det_idx] > max_iou_distance || cost_matrix[track_idx][det_idx] < 0){
            mresult.unmatched_tracks.push_back(iou_track_candidates[track_idx]);
        }
        else{
            mresult.add_matched(iou_track_candidates[track_idx], detections[det_idx]);
            confirmed_det_idxs.insert(det_idx);
        }
    }

    for (int i = 0; i < detections.size(); i++){
        if (confirmed_det_idxs.count(i)==0){
            mresult.unmatched_detections.push_back(detections[i]);
        }
    }

}
