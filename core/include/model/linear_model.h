//
// Created by qfeng on 17-5-25.
//

#ifndef PCTR_LINEAR_MODEL_H
#define PCTR_LINEAR_MODEL_H

#include "model.h"
#include <Eigen/Sparse>
#include <memory>
#include <boost/optional.hpp>
#include <spdlog/logger.h>

using boost::optional;

using namespace std;

class linear_model : public model {
public:
    shared_ptr<spdlog::logger> _logger;

    Eigen::SparseVector<float> _weights;

    linear_model(uint64_t weight_size);

    virtual ~linear_model();

    uint64_t weight_size();

    float get(uint64_t index);

    void update_weight(uint64_t index, float value);

    float margin(const Eigen::SparseVector<float>& sample);

    float logistic_predict(const Eigen::SparseVector<float>& sample);

    static shared_ptr<linear_model> load_model(const string& model_path);
};


#endif //PCTR_LINEAR_MODEL_H
