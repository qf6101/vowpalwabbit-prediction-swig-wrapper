#pragma once

#include <unordered_map>
#include <valarray>
#include <string>
#include <boost/optional.hpp>
#include <Eigen/Core>
#include <spdlog/logger.h>

using std::string;
// using DenseVector = std::valarray<float>;
using DenseVector = Eigen::VectorXf;
using boost::optional;
using boost::none;

class word2vec_model {
public:
    std::shared_ptr<spdlog::logger> _logger;

    typedef std::unordered_map<string, DenseVector> EmbeddingMap;
    EmbeddingMap _embeddings;

    word2vec_model(EmbeddingMap&& embeddings);

    static std::unique_ptr<word2vec_model> load_model(string model_path);

    int dimension() const { return _embeddings.begin()->second.size(); }

    optional<DenseVector> get(const string &key) const {
        auto p = _embeddings.find(key);
        if (p != _embeddings.end())
            return p->second;
        else
            return none;
    }
};