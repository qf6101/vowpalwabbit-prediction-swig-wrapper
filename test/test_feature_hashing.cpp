//
// Created by qfeng on 17-5-22.
//

#include <gtest/gtest.h>
#include <feature/feature_hashing.h>

namespace ctr_test {

    class test_feature_hashing : public ::testing::Test {
    };

    TEST_F(test_feature_hashing, hash_contant) {
        EXPECT_EQ(feature_hashing::hash_constant(), 116060);
    }

    TEST_F(test_feature_hashing, hash_integer_feature) {
        EXPECT_EQ(feature_hashing::hash_single_feature("", "123456789"), 249109);
    }

    TEST_F(test_feature_hashing, hash_string_feature) {
        EXPECT_EQ(feature_hashing::hash_single_feature("", "has_car"), 36073);
    }

    TEST_F(test_feature_hashing, hash_namespace) {
        EXPECT_EQ(feature_hashing::hash_namespace("gender"), 2678092942);
    }

    TEST_F(test_feature_hashing, hash_string_feature_with_namespace_1) {
        EXPECT_EQ(feature_hashing::hash_single_feature("gender", "男"), 27805);
    }

    TEST_F(test_feature_hashing, hash_string_feature_with_namespace_2) {
        EXPECT_EQ(feature_hashing::hash_single_feature("interest", "篮球"), 61583);
    }

    TEST_F(test_feature_hashing, hash_string_feature_with_namespace_3) {
        EXPECT_EQ(feature_hashing::hash_single_feature("age", "42"), 165243);
    }

    TEST_F(test_feature_hashing, hash_quadratic_feature_1) {
        EXPECT_EQ(feature_hashing::hash_quadratic_features("gender", "男", "interest", "篮球"), 134056);
    }

    TEST_F(test_feature_hashing, hash_quadratic_feature_2) {
        EXPECT_EQ(feature_hashing::hash_quadratic_features("gender", "男", "gender", "男"), 169914);
    }

    TEST_F(test_feature_hashing, hash_quadratic_feature_3) {
        EXPECT_EQ(feature_hashing::hash_quadratic_features("interest", "篮球", "interest", "篮球"), 147858);
    }

} //namespace

