#include <catch2/catch_test_macros.hpp>
#include "WorldRegistry.hpp"

struct Velocity {
  int x;
  int y;
};

struct Acceleration {
  int x;
  int y;
};

struct Speed {
  int x;
  int y;
  int z;
};

TEST_CASE("Component count", "[component_count]") {
  WorldRegistry registry {10};
  REQUIRE(registry.count_components() == 0);
  ComponentId id_vel = registry.register_component<Velocity>();
  REQUIRE(registry.count_components() == 1);
  ComponentId id_speed = registry.register_component<Speed>();
  REQUIRE(registry.count_components() == 2);
  ComponentId id_acceleration = registry.register_component<Acceleration>();
  REQUIRE(registry.count_components() == 3);
}

TEST_CASE("Component id", "[component_id]") {
  WorldRegistry registry {10};
  ComponentId id_vel = registry.register_component<Velocity>();
  ComponentId id_speed = registry.register_component<Speed>();
  ComponentId id_acceleration = registry.register_component<Acceleration>();
  REQUIRE(id_vel != id_speed);
  REQUIRE(id_vel != id_acceleration);
  REQUIRE(id_speed != id_acceleration);
}

TEST_CASE("Entity Id", "[entity_id]") {
  WorldRegistry registry {10};
  registry.register_component<Velocity>();
  registry.register_archetype<Velocity>();
  EntityId list [10];
  for (int i = 0; i < 10; ++i) {
    list[i] = registry.create_entity<Velocity>();
  }
  REQUIRE(registry.get_id() == 11);
}

TEST_CASE("Register entity without archetype", "[archetypeless_registering]") {
  WorldRegistry registry {10};
  registry.register_component<Velocity>();
  EntityId list [10];
  for (int i = 0; i < 10; ++i) {
    list[i] = registry.create_entity<Velocity>();
  }
  REQUIRE(registry.get_id() == 11);
}

TEST_CASE("Id component retrieval from type", "[component_id_retrieval_from_type]") {
  WorldRegistry registry { 10 };
  ComponentId id_vel = registry.register_component<Velocity>();
  ComponentId id_speed = registry.register_component<Speed>();
  ComponentId id_acceleration = registry.register_component<Acceleration>();
  REQUIRE(registry.get_component_id<Velocity>() == id_vel);
  REQUIRE(registry.get_component_id<Speed>() == id_speed);
  REQUIRE(registry.get_component_id<Acceleration>() == id_acceleration);
}

TEST_CASE("Archetype Id", "[archetype_id]") {
  WorldRegistry registry {10};
  ComponentId id_vel = registry.register_component<Velocity>();
  ComponentId id_speed = registry.register_component<Speed>();
  ComponentId id_acceleration = registry.register_component<Acceleration>();
  auto vel_speed_arch = registry.register_archetype<Velocity, Speed>().value();
  auto speed_arch = registry.register_archetype<Speed>().value();
  auto speed_acc_arch = registry.register_archetype<Speed, Acceleration>().value();
  REQUIRE(id_vel != id_speed);
  REQUIRE(id_speed != id_acceleration);
  REQUIRE(id_acceleration != id_vel);
  REQUIRE(vel_speed_arch == registry.get_archetype_id<Velocity, Speed>());
  REQUIRE(speed_acc_arch == registry.get_archetype_id<Speed, Acceleration>());
  REQUIRE(speed_arch == registry.get_archetype_id<Speed>());
}

TEST_CASE("Find Component", "[component_entity]") {
  WorldRegistry registry { 10 };
  ComponentId id_vel = registry.register_component<Velocity>();
  EntityId entity = registry.create_entity<Velocity>();
  registry.add_component(entity, id_vel);
  Velocity velocity = Velocity {10, 20};
  registry.attach_component<Velocity>(entity, (Velocity){10, 20});
  Velocity component_from_registry = registry.get_component<Velocity>(entity).value();
  REQUIRE(component_from_registry.x == 10);
  REQUIRE(component_from_registry.y == 20);
}
