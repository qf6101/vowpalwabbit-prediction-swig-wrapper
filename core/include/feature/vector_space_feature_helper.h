#pragma once

#include <vector>
#include <utility>
#include <boost/optional.hpp>
#include <Eigen/SparseCore>
#include "model/word2vec_model.h"
using boost::optional;
using boost::none;
typedef std::vector<std::pair<std::string, float>> WordWeightList;
using SparseVector = Eigen::SparseVector<float>;

const float float_nan = std::numeric_limits<float>::quiet_NaN();

optional<DenseVector> average_word2vec_vector(const word2vec_model& w2v_model, const WordWeightList& l);
int murmur3_hash_string(const std::string& x, int seed);
int hash_string(const std::string& x, int size);
optional<SparseVector> word_weight_to_sparse_vector(const WordWeightList& l, int hash_size);

template<typename T>
optional<T> average_vec(std::vector<T> vecs) {
  if(vecs.empty())
    return none;
  else {
    bool first = true;
    T res;
    for(auto v: vecs){
      if(first){
        res = v;
        first = false;
      }
      else {
        res += v;
      }
    }
    res /= (float) vecs.size();
    return res;
  }
}

template<typename T>
float optional_cosine_similarity(optional<T> x, optional<T> y) {
  if(x && y){
    auto x_ = x.value(), y_ = y.value();
    return x_.dot(y_)/std::sqrt((x_.dot(x_)*y_.dot(y_)));
  }
  else
    return float_nan;
}

float jaccard_similarity(const WordWeightList& x, const WordWeightList& y, float min_weight);