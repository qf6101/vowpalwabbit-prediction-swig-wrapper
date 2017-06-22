#pragma once

#include "features.h"
#include "attribute/user_profile.h"
#include "attribute/context_info.h"
#include "attribute/doc_attributes.h"
#include "model/word2vec_model.h"

class feature_engineering_user_doc_similarity {
public:
    const int hash_size;
    const float jaccard_min_weight;
    const word2vec_model& w2v_model;
    feature_engineering_user_doc_similarity(int hash_size, float jaccard_min_weight, const word2vec_model& w2v_model)
        : hash_size(hash_size), jaccard_min_weight(jaccard_min_weight), w2v_model(w2v_model) {};
    void fill_features(features& f, const user_profile& user, const context_info& cxt, const doc_attributes& doc);
};