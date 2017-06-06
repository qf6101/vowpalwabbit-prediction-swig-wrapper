//
// Created by qfeng on 17-5-22.
//

#include <cstdint>
#include "feature/feature_hashing.h"
#include "vw_hashing/constant.h"
#include "vw_hashing/hash.h"

uint64_t feature_hashing::size() {
    return _size;
}

uint64_t feature_hashing::hash_constant() {
    return bias_constant % (1 << _b);
}

uint64_t feature_hashing::hash_namespace(string ns) {
    if (ns.size() > 0) {
        return hash_string(ns, hash_base);
    } else return 0;
}

uint64_t feature_hashing::hash_single_feature(string ns, string feature) {
    if (feature.size() > 0) {
        uint64_t ns_hash = hash_namespace(ns);
        return hash_string(feature, ns_hash) % (1 << _b);
    } else return 0;
}

uint64_t feature_hashing::hash_quadratic_features(string lhs_ns,
                                                  string lhs_feature,
                                                  string rhs_ns,
                                                  string rhs_feature) {
    uint64_t lhs_hash = hash_single_feature(lhs_ns, lhs_feature);
    uint64_t rhs_hash = hash_single_feature(rhs_ns, rhs_feature);
    return ((lhs_hash * FNV_prime) ^ rhs_hash) % (1 << _b);
}

uint64_t feature_hashing::hash_string(string s, uint64_t h) {
    std::basic_string<char, std::char_traits<char>, std::allocator<char>>::iterator iter = s.begin();
    //trim leading whitespace but not UTF-8
    for (; iter < s.end() && *iter <= 0x20 && (int) *iter >= 0; iter++);
    iter = s.end();
    //trim trailing white space but not UTF-8
    for (; iter > s.begin() && *(iter - 1) <= 0x20 && (int) *(iter - 1) >= 0; iter--);

    size_t ret = 0;
    iter = s.begin();
    while (iter != s.end())
        if (*iter >= '0' && *iter <= '9')
            ret = 10 * ret + *(iter++) - '0';
        else
            return uniform_hash((unsigned char *) s.c_str(), s.length(), h);

    return ret + h;
}