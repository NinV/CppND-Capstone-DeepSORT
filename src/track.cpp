#include "track.h"

Track::Track(TrackValue &_track_value, int _trackId, int _n_init, int _max_age){
    trackId_ = _trackId;
    n_init_ = _n_init;
    max_age_ = _max_age;
    track_value_ = _track_value;
    calc_box();
}

void Track::calc_box() {
    float width = track_value_.state_vector[A] * track_value_.state_vector[H];
    box_ = cv::Rect(int(track_value_.state_vector[Cx] - width / 2),
                    int(track_value_.state_vector[Cy] - track_value_.state_vector[H] / 2),
                    int(width),
                    int(track_value_.state_vector[H])
            );
}

StateVector & Track::state_vector() {return track_value_.state_vector;}
StateCovariance & Track::covariance() { return track_value_.covariance;}
