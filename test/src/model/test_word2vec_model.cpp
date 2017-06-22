#include <gtest/gtest.h>
#include <model/word2vec_model.h>
#include <Eigen/Core>
#include <ios>

namespace ctr_test {
    TEST(test_word2vec_model, load) {
        auto model = word2vec_model::load_model("test/data/word2vec_model.txt");
        EXPECT_TRUE(model);
        EXPECT_EQ(model.get()->_embeddings.size(), 3);
        const auto& vec = model.get()->_embeddings.at("ming");
        // when valarray<flaot> as DenseVector
        // EXPECT_TRUE((vec == DenseVector({0.096344, -0.14804, -0.29072, -0.088793})).min());
        EXPECT_EQ(vec, Eigen::Vector4f({0.096344, -0.14804, -0.29072, -0.088793}));
        EXPECT_EQ(model.get()->dimension(), 4);
        model = word2vec_model::load_model("test/data/word2vec_model.bad.txt");
        EXPECT_TRUE(!model);
        model = word2vec_model::load_model("test/data/word2vec_model.no_such_file.txt");
        EXPECT_TRUE(!model);
    }
}
