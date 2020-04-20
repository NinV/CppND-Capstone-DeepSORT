//
// Created by ninv on 4/20/20.
//

#include "track.h"

Track::Track(float mean, float covariance, int trackId, int n_init, int max_age){
    mean_ = mean;
    covariance_ = covariance;
    trackId_ = trackId;
    n_init_ = n_init;
    max_age_ = max_age;
}

Track::Track(float mean, float covariance, int trackId, int n_init, int max_age, std::vector<float> *feature){
    mean_ = mean;
    covariance_ = covariance;
    trackId_ = trackId;
    n_init_ = n_init;
    max_age_ = max_age;
    if (feature!= nullptr){
        feature_ = feature;
    }
}
