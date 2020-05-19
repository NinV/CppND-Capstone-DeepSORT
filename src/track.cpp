#include "track.h"

Track::Track(TrackValue &_track_value, int _trackId, int _n_init, int _max_age){
    trackId_ = _trackId;
    n_init_ = _n_init;
    max_age_ = _max_age;
    track_value_ = _track_value;

}

cv::Rect Track::box() {
    double width = track_value_.state_vector[A] * track_value_.state_vector[H];
    return cv::Rect(int(track_value_.state_vector[Cx] - width / 2),
                    int(track_value_.state_vector[Cy] - track_value_.state_vector[H] / 2),
                    int(width),
                    int(track_value_.state_vector[H])
            );
}

void Track::predict(KalmanFilter &kf) {
    kf.predict(track_value_);
    age_ += 1;
    time_since_update_ += 1;
}

void Track::update(KalmanFilter &kf, Detection &det) {
    Measurement m = det.to_xyah();
    kf.update(track_value_, m);

    hits_ += 1;
    time_since_update_ = 0;
    if (state_ == Tentative && hits_ > n_init_){
        state_ = Confirmed;
    }
}

void Track::mark_missed() {
    if (state_ == Tentative || time_since_update_ > max_age_){state_ = Deleted;}
//    else if (time_since_update_ > max_age_) {state_ = Deleted;}
}

vector<double> Track::to_tlbr() {
    double width = track_value_.state_vector[A] * track_value_.state_vector[H];
    double t = track_value_.state_vector[Cy] - track_value_.state_vector[H] / 2;
    double l = track_value_.state_vector[Cx] - width / 2;
    double b = t + track_value_.state_vector[H];
    double r = l + width;

    vector<double> tlbr{t, l, b, r};
    return tlbr;
}
