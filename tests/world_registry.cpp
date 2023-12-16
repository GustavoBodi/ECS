#include <catch2/catch_test_macros.hpp>
#include "WorldRegistry.hpp"

struct Velocity {
  int x;
  int y;
};

struct Speed {
  int x;
  int y;
  int z;
};

TEST_CASE("Registry Id", "[id]") {
  WorldRegistry registry {};
  REQUIRE(registry.count_components() == 0);
  registry.register_component<Velocity>();
  REQUIRE(registry.count_components() == 1);
  registry.register_component<Speed>();
  REQUIRE(registry.count_components() == 2);
}
