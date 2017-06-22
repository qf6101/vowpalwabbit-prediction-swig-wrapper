#include <gtest/gtest.h>
#include "feature/vector_space_feature_helper.h"

namespace ctr_test {
  TEST(test_murmurhash3, ordinary_hash) {
    EXPECT_EQ(murmur3_hash_string("中国", 42), 1009927282);
    EXPECT_EQ(murmur3_hash_string("America", 0), -328147168);
  }
}