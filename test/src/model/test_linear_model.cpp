//
// Created by qfeng on 17-5-31.
//

#include <gtest/gtest.h>
#include "model/linear_model.h"

namespace ctr_test {

    class test_linear_model: public ::testing::Test {
    protected:
        unique_ptr<linear_model> model;

        virtual void SetUp() {
            model.reset(new linear_model(pow(2, 18)));
        }

        virtual void TearDown() {
        }
    };


    TEST_F(test_linear_model, logistic_predict) {
        model->update_weight(0, 0.032);
        model->update_weight(1, 0.125);
        model->update_weight(3, -0.0265);
        model->update_weight(6, 0);
        Eigen::SparseVector<float> sample(pow(2, 18));
        sample.coeffRef(0) = 0.683;
        sample.coeffRef(1) = -0.0234;
        sample.coeffRef(2) = 0.435;
        sample.coeffRef(3) = 0.112;
        sample.coeffRef(4) = -0.087;
        sample.coeffRef(5) = 0.123;
        sample.coeffRef(6) = -0.1345;
        sample.coeffRef(8) = 0.1;
        float predicted = model->logistic_predict(sample);
        EXPECT_NEAR(predicted, 0.503990665259458, 1E-7);
    }
} //namespace