//
// Created by qfeng on 17-5-23.
//

#ifndef PCTR_CTR_PREDICTION_H
#define PCTR_CTR_PREDICTION_H


#include "feature/feature_engineering_user_doc_similarity.h"
#include "feature/feature_engineering_recent_docs_similarity.h"
#include "attribute/user_profile.h"
#include "attribute/context_info.h"
#include "attribute/doc_attributes.h"
#include "model/model_daemon.h"
#include <memory>


class linear_predictor {
public:
    // spdlog logger
    shared_ptr<spdlog::logger> _logger;
    // swig does not support unique_ptr
    model_daemon* _daemon = nullptr;
    feature_engineering_user_doc_similarity* _fe_user_doc_sim = nullptr;
    feature_engineering_recent_docs_similarity* _fe_recent_docs_sim = nullptr;
    bool _started = false;

    linear_predictor();
    linear_predictor(const string& model_path, const string& w2v_model_path, int update_interval = 30);
    virtual ~linear_predictor();
    float logistic_predict (const user_profile& user, const context_info& cxt, const doc_attributes& doc) const;
    float test (const user_profile& user, const context_info& cxt, const doc_attributes& doc) const;
};


#endif //PCTR_CTR_PREDICTION_H
