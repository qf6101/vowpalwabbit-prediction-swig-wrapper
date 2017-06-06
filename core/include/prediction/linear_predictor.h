//
// Created by qfeng on 17-5-23.
//

#ifndef PCTR_CTR_PREDICTION_H
#define PCTR_CTR_PREDICTION_H


#include "attribute/user_profile.h"
#include "attribute/context_info.h"
#include "attribute/doc_attributes.h"
#include "model/model_daemon.h"

class linear_predictor {
public:
    model_daemon* _daemon = NULL;
    linear_predictor(string model_path, int update_interval = 30);
    virtual ~linear_predictor();
    float logistic_predict(user_profile& user, context_info& cxt, doc_attributes& doc);
    float test(user_profile& user, context_info& cxt, doc_attributes& doc);
};


#endif //PCTR_CTR_PREDICTION_H
