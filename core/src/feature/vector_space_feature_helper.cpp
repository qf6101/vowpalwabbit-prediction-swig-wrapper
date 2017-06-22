#include "feature/vector_space_feature_helper.h"
#include "murmurhash3/MurmurHash3.h"

optional<DenseVector> average_word2vec_vector(const word2vec_model &w2v_model, const WordWeightList &items) {
    DenseVector res(w2v_model.dimension());
    float total_weight = 0.0f;
    bool no_value = true;
    for (auto x: items) {
        if (auto p = w2v_model.get(x.first)) {
            if (no_value) {
                res = p.value() * x.second;
                no_value = false;
            } else {
                res += p.value() * x.second;
            }
            total_weight += x.second;
        }
    }
    if (no_value)
        return none;
    else {
        res /= total_weight;
        return res;
    }
}

int murmur3_hash_string(const std::string &x, int seed) {
    uint32_t res;
    MurmurHash3_x86_32_spark(x.c_str(), x.size(), seed, &res);
    return res;
}

int hash_string(const std::string &x, int size) {
    const int seed = 42;
    int raw_hash = murmur3_hash_string(x, seed);
    if (raw_hash > 0) return raw_hash % size; else return (raw_hash % size) + size;
}

optional<SparseVector> word_weight_to_sparse_vector(const WordWeightList &l, int hash_size) {
    if (l.empty())
        return none;
    else {
        SparseVector res(hash_size);
        for (auto item: l) {
            res.coeffRef(hash_string(item.first, hash_size)) += item.second;
        }
        return res;
    }
}

float jaccard_similarity(const WordWeightList &x, const WordWeightList &y, float min_weight) {
    std::vector<std::string> wx;
    std::vector<std::string> wy;
    for (auto i: x) if (i.second >= min_weight) wx.push_back(i.first);
    for (auto i: y) if (i.second >= min_weight) wy.push_back(i.first);
    std::sort(wx.begin(), wx.end());
    std::sort(wy.begin(), wy.end());

    std::vector<std::string> w_intersection;
    std::set_intersection(wx.begin(), wx.end(), wy.begin(), wy.end(), std::back_inserter(w_intersection));
    return float(w_intersection.size()) / float(wx.size() + wy.size() - w_intersection.size());
}