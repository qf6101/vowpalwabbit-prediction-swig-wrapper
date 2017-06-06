//
// Created by qfeng on 17-5-22.
//

#ifndef PCTR_FEATURE_HASHING_H
#define PCTR_FEATURE_HASHING_H

#include <cstdint>
#include <string>

using std::string;

class feature_hashing {
private:
    static const uint32_t _b = 18;
    static const uint64_t _size = 1 << _b;
    static uint64_t hash_string(string s, uint64_t h);
public:
    static uint64_t size();
    static uint64_t hash_constant();
    static uint64_t hash_namespace(string ns);
    static uint64_t hash_single_feature(string ns, string feature);
    static uint64_t hash_quadratic_features(string lhs_ns, string lhs_feature,
                                     string rhs_ns, string rhs_feature);
};

#endif //PCTR_FEATURE_HASHING_H
