//
// Created by qfeng on 17-5-23.
//

#include <feature/features.h>
#include <log/pctr_log.h>
#include "prediction/linear_predictor.h"
#include "feature/feature_engineering_base.h"
#include "feature/feature_engineering_time.h"
#include "feature/feature_engineering_location.h"

linear_predictor::linear_predictor() {
    _logger = spdlog::get(pctr_log::_core_logger_name);
    _logger->info("Initialize linear predictor.");
}

linear_predictor::linear_predictor(const string &model_path, const string &w2v_model_path, int update_interval)
        : linear_predictor() {
    _daemon = new model_daemon(model_path, "linear_model", w2v_model_path, update_interval);
    if (_daemon->start()) {
        _logger->info("Finish starting model daemon with model path={}, word2vec path={} and update interval={}.",
                      model_path, w2v_model_path, update_interval);
        const unsigned int hash_size = _daemon->_model->weight_size();
        const float jaccard_min_weight = 0.1;
        _fe_user_doc_sim =
                new feature_engineering_user_doc_similarity(hash_size, jaccard_min_weight, *_daemon->_w2v_model);
        _fe_recent_docs_sim =
                new feature_engineering_recent_docs_similarity(hash_size, jaccard_min_weight, *_daemon->_w2v_model);
        _logger->info("Finish initializing feature engineering with hash_size={} and jaccard_min_weight={}", hash_size,
                      jaccard_min_weight);
        _started = true;
    } else {
        _logger->info("Fail to start model daemon with model path={}, word2vec path={} and update interval={}.",
                      model_path, w2v_model_path, update_interval);
    }
}

linear_predictor::~linear_predictor() {
    if (_daemon != nullptr) _daemon->stop(), delete _daemon;
    if (_fe_user_doc_sim != nullptr) delete _fe_user_doc_sim;
    if (_fe_recent_docs_sim != nullptr) delete _fe_recent_docs_sim;
    _started = false;
    _logger->info("Leave linear predictor.");
}

float
linear_predictor::logistic_predict(const user_profile &user, const context_info &cxt, const doc_attributes &doc) const {
    try {
        // fill features by feature engineering
        features f(*_daemon->_hash);
        feature_engineering_base::fill_features(f, user, cxt, doc);
        feature_engineering_time::fill_features(f, user, cxt, doc);
        feature_engineering_location::fill_features(f, user, cxt, doc);
        _fe_user_doc_sim->fill_features(f, user, cxt, doc);
        _fe_recent_docs_sim->fill_features(f, user, cxt, doc);
        // encode features with vowpal wabbit feature hash and predict score
        if (auto vw_features = std::move(f.to_vw())) return _daemon->logistic_predict(*vw_features);
        else return 0;
    } catch (exception &ex) {
        _logger->debug("Fail to predict score with user id={}, doc id={}, detail: {}.",
                       user.user_id, doc.doc_id, ex.what());
        return 0;
    }
}

float linear_predictor::test(const user_profile &user, const context_info &cxt, const doc_attributes &doc) const {
    _logger->debug("Trigger test in linear predictor.");
    return cxt.rdocs[0].time;
}