//
// Created by qfeng on 17-5-22.
//

#ifndef PCTR_FEATURE_HASHING_H
#define PCTR_FEATURE_HASHING_H

#include <cstdint>
#include <string>

using std::string;

class feature_hash {
private:
    uint64_t _size;
public:
    feature_hash(uint64_t size);
    uint64_t size() const;
    bool resize(uint64_t size);
    uint64_t hash_string(const string& s, uint64_t h) const;
    uint64_t hash_constant() const;
    uint64_t hash_namespace(const string& ns) const;
    uint64_t hash_single_feature(const string& ns, const string& feature) const;
    uint64_t hash_quadratic_features(const string& lhs_ns, const string& lhs_feature,
                                     const string& rhs_ns, const string& rhs_feature) const;
};

#endif //PCTR_FEATURE_HASHING_H
