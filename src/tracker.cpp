#include "tracker.h"

void Tracker::predict() {
    for(Track track:tracks_){track.predict(kf_);}
}

void Tracker::update(vector <Detection> &detections) {
    MatchResult m = match(detections);

    for(int i=0; i < m.matched_tracks.size(); i++){
        m.matched_tracks[i].update(kf_, m.matched_detections[i]);

    }

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
    return MatchResult();
}
