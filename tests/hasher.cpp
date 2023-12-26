#include <catch2/catch_test_macros.hpp>
#include "Hasher.hpp"

TEST_CASE("Vector Hashing correctness", "[tuple_hash]") {
  VectorHasher<uint64_t> hasher;
  std::vector<uint64_t> vec1 {1, 4, 6, 3, 2};
  std::vector<uint64_t> vec2 {1, 4, 6, 3, 2};
  std::vector<uint64_t> vec3 {1, 4, 6, 3, 2, 10};
  std::vector<uint64_t> vec4 {1, 4, 6, 3, 2, 10};
  std::vector<uint64_t> vec5 {7, 4, 1, 76, 2, 10};
  auto hash1 = hasher(vec1);
  auto hash2 = hasher(vec2);
  auto hash3 = hasher(vec3);
  auto hash4 = hasher(vec4);
  auto hash5 = hasher(vec5);
  REQUIRE(hash1 == hash2);
  REQUIRE(hash3 == hash4);
  REQUIRE(hash4 != hash5);
  REQUIRE(hash1 != hash4);
  REQUIRE(hash1 != hash5);
}

