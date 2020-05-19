#include "tracker.h"


int Tracker::next_track_idx() {
    total_tracks += 1;
    return total_tracks;
}

void Tracker::initiate_track(TrackValue tv) {
    int trackId = next_track_idx();
    shared_ptr<Track> t = make_shared<Track>(tv, trackId, n_init, max_age);
    tracks_.insert({trackId, t});
}

void Tracker::remove_track(int trackId) {
    tracks_.erase(trackId);
}

void Tracker::predict() {
    for (auto& [trackId, t]: tracks_) {
        t->predict(kf_);
    }
}

void Tracker::reset_matched() {
    for(auto& [trackId, t]:tracks_){t->mstate=Unmatched;}
}

vector<pair<shared_ptr<Track>, Detection*>> Tracker::match(vector<Detection> &detections) {
    vector<pair<shared_ptr<Track>, Detection*>> pairs;
    if (tracks_.empty() || detections.empty()){ return pairs;}

    cout << "Start matching\n";
    // use IOU for matching
    // create cost matrix for iou matching
    vector<vector<double>> t_boxes;
    vector<vector<double>> det_boxes;
    for (Detection &det: detections){
        det_boxes.push_back(det.to_tlbr());
    }

    vector<int> tIds;   // to retrieve the trackId back
    for (auto& [trackId, t]:tracks_){
        t_boxes.push_back(t->to_tlbr());
        tIds.push_back(trackId);
    }
    auto cost_matrix = iou_cost(t_boxes, det_boxes, max_iou_distance);

    // matching Track instances to Detection instances using Hungarian (Munkres) algorithm
    HungarianAlgorithm HungAlgo;
    vector<int> assignments;

    HungAlgo.Solve(cost_matrix, assignments);

    for (int i = 0; i < cost_matrix.size(); i++){
        int det_idx = assignments[i];
        int trackID = tIds[i];

//        cout << "Track box: " <<tracks_[trackID]->box() << endl;
//        cout << "Det box: " << detections[det_idx].box << endl;
//        cout << "iou cost: " << cost_matrix[i][det_idx] << endl;
        // mark track that dont have any matches or min iou distance is too large into Unmatched
        if (cost_matrix[i][det_idx] > max_iou_distance || cost_matrix[i][det_idx] < 0){
            tracks_[trackID]->mstate = Unmatched;
//            continue;
        }
        else{
            tracks_[trackID]->mstate = Matched;
            detections[det_idx].mstate = Matched;
            pairs.emplace_back(pair<shared_ptr<Track>, Detection*>{tracks_[trackID], &detections[det_idx]});
        }
    }

    return pairs;
}


void Tracker::update(vector <Detection> &detections) {
    reset_matched();

    vector<pair<shared_ptr<Track>, Detection*>> pairs = match(detections);

    // updated matched pairs of Track and Detection
    for (auto &[t_ptr, det_ptr] : pairs){
        t_ptr->update(kf_, *det_ptr);
    }

    // mark missed for tentative tracks and confirmed track that doesn't matched
    for (auto &[trackId, t_ptr] : tracks_){
        if (t_ptr->mstate == Unmatched){t_ptr->mark_missed();}
    }

    // remove tracks are marked as Deleted
    for (auto &[trackId, t] : tracks_){
        if (t->is_deleted()){
            std::cout << "remove trackID" << t->trackID();
            remove_track(trackId);
            std::cout << " # tracks: " << tracks_.size() << "\n";
        }
    }

    // initiate unmatched detection
    for (auto &det : detections){
        if(det.mstate == Unmatched){
            TrackValue tv = kf_.initiate(det.to_xyah());
            this->initiate_track(tv);}
            std::cout << "Initiate a track object with trachID = " << (--tracks_.end())->first << "\n";
    }
}

vector<shared_ptr<Track>> Tracker::tracks() {
    vector<shared_ptr<Track>> t;
    for(auto &[_, t_ptr]: tracks_){
        t.push_back(t_ptr);
    }
    return t;
}

