//
// Created by qfeng on 17-6-6.
//

#ifndef PCTR_MODEL_H
#define PCTR_MODEL_H

#include <Eigen/Sparse>


class model {
public:
    virtual float logistic_predict(Eigen::SparseVector<float>& sample) = 0;
    virtual ~model() {};
};


#endif //PCTR_MODEL_H
