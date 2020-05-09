#include "track.h"

Track::Track(TrackValue &_track_value, int _trackId, int _n_init, int _max_age){
    trackId_ = _trackId;
    n_init_ = _n_init;
    max_age_ = _max_age;
    track_value_ = _track_value;
    calc_box();
}

void Track::calc_box() {
    float width = track_value_.mean[A] * track_value_.mean[H];
    box_ = cv::Rect(int(track_value_.mean[Cx] - width / 2),
                    int(track_value_.mean[Cy] - track_value_.mean[H] / 2),
                    int(width),
                    int(track_value_.mean[H])
            );
}

MeanTrackValue & Track::mean() {return track_value_.mean}
CovarianceTrackValue & Track::covariance() { return track_value_.covariance}
