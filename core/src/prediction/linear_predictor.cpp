//
// Created by qfeng on 17-5-23.
//

#include <feature/features.h>
#include "prediction/linear_predictor.h"
#include "feature/feature_engineering_base.h"
#include "feature/feature_engineering_time.h"
#include "feature/feature_engineering_location.h"

linear_predictor::linear_predictor(string model_path, int update_interval) {
    _daemon = new model_daemon(model_path, "linear_model", update_interval);
    _daemon->start();
}

linear_predictor::~linear_predictor() {
    if (_daemon != NULL) {
        _daemon->stop();
        delete _daemon;
    }
}

float linear_predictor::logistic_predict(user_profile& user, context_info& cxt, doc_attributes& doc) {
    features f;

    feature_engineering_base::fill_features(f, user, cxt, doc);
    feature_engineering_time::fill_features(f, user, cxt, doc);
    feature_engineering_location::fill_features(f, user, cxt, doc);

    Eigen::SparseVector<float> *vw_features = NULL;
    f.to_vw(&vw_features);
    float score = _daemon->logistic_predict(*vw_features);
    if (vw_features != NULL) delete vw_features;
    return score;
}

float linear_predictor::test(user_profile& user, context_info& cxt, doc_attributes& doc) {
    return cxt.time;
}