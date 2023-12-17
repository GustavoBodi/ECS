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

TEST_CASE("Component Id", "[component_id]") {
  WorldRegistry registry {10};
  REQUIRE(registry.count_components() == 0);
  registry.register_component<Velocity>();
  REQUIRE(registry.count_components() == 1);
  registry.register_component<Speed>();
  REQUIRE(registry.count_components() == 2);
}

TEST_CASE("Entity Id", "[entity_id]") {
  WorldRegistry registry {10};
  EntityId list [10];
  for (int i = 0; i < 10; ++i) {
    list[i] = registry.create_entity<>();
  }
  REQUIRE(registry.get_id() == 10);
}
