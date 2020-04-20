#include <vector>

#ifndef CPPND_CAPSTONE_DEEPSORT_TRACK_H
#define CPPND_CAPSTONE_DEEPSORT_TRACK_H

enum TrackState { Tentative, Confirmed, Deleted };

class Track {
private:
    float mean_{}, covariance_{};
    int trackId_{}, max_age_{}, n_init_{};
    int hits_=1, age_=1, time_since_update_=0;
    TrackState state_ = Tentative;
    std::vector<float> *feature_= nullptr;
public:
    Track(float mean, float covariance, int trackId, int n_init, int max_age);
    Track(float mean, float covariance, int trackId, int n_init, int max_age, std::vector<float> *feature);
};

/*
    def __init__(self, mean, covariance, track_id, n_init, max_age,
                 feature=None):
        self.mean = mean
        self.covariance = covariance
        self.track_id = track_id
        self.hits = 1
        self.age = 1
        self.time_since_update = 0

        self.state = TrackState.Tentative
        self.features = []
        if feature is not None:
            self.features.append(feature)

        self._n_init = n_init
        self._max_age = max_age
 * */


#endif //CPPND_CAPSTONE_DEEPSORT_TRACK_H
