//
// Created by qfeng on 17-5-25.
//

#ifndef PCTR_LINEAR_MODEL_H
#define PCTR_LINEAR_MODEL_H

#include "model.h"
#include <Eigen/Sparse>

using namespace std;

class linear_model: public model {
public:
    Eigen::SparseVector<float> _weights;

    linear_model(uint64_t weight_size);

    virtual ~linear_model();

    uint64_t weight_size();

    float get(uint64_t index);

    void update_weight(uint64_t index, float value);

    float margin(Eigen::SparseVector<float>& sample);

    float logistic_predict(Eigen::SparseVector<float>& sample);

    static bool load_model(string model_path, linear_model **output_model);
};


#endif //PCTR_LINEAR_MODEL_H
