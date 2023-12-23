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

TEST_CASE("Find Component (Single Component archetype)", "[single_component_retrieval]") {
  WorldRegistry registry { 10 };

  ComponentId id_vel = registry.register_component<Velocity>();
  ComponentId id_acc = registry.register_component<Acceleration>();

  EntityId entity = registry.create_entity<Velocity>();
  EntityId entity2 = registry.create_entity<Velocity>();
  EntityId entity3 = registry.create_entity<Acceleration>();
  EntityId entity4 = registry.create_entity<Acceleration>();
  EntityId entity5 = registry.create_entity<Acceleration, Velocity>();

  registry.attach_component<Velocity>(entity, (Velocity){10, 20});
  registry.attach_component<Velocity>(entity2, (Velocity){20, 30});
  registry.attach_component<Acceleration>(entity3, (Acceleration){40, 50});
  registry.attach_component<Acceleration>(entity4, (Acceleration){60, 10});

  Velocity component_from_registry_1 = registry.get_component<Velocity>(entity).value();
  REQUIRE(component_from_registry_1.x == 10);
  REQUIRE(component_from_registry_1.y == 20);
  Velocity component_from_registry_2 = registry.get_component<Velocity>(entity2).value();
  REQUIRE(component_from_registry_2.x == 20);
  REQUIRE(component_from_registry_2.y == 30);
  Acceleration component_from_registry_3 = registry.get_component<Acceleration>(entity3).value();
  REQUIRE(component_from_registry_3.x == 40);
  REQUIRE(component_from_registry_3.y == 50);
  Acceleration component_from_registry_4 = registry.get_component<Acceleration>(entity4).value();
  REQUIRE(component_from_registry_4.x == 60);
  REQUIRE(component_from_registry_4.y == 10);
}

TEST_CASE("Find component (Multiple component archetype)", "[multiple_component_retrieval]") {
  WorldRegistry registry { 10 };
  ComponentId id_vel = registry.register_component<Velocity>();
  ComponentId id_acc = registry.register_component<Acceleration>();

  EntityId entity { registry.create_entity<Acceleration, Velocity>() };
  EntityId entity2 { registry.create_entity<Acceleration, Velocity>() };
  registry.attach_component<Velocity>(entity, (Velocity){10, 20});
  registry.attach_component<Acceleration>(entity, (Acceleration){20, 70});
  registry.attach_component<Velocity>(entity2, (Velocity){50, 10});
  registry.attach_component<Acceleration>(entity2, (Acceleration){80, 90});

  Acceleration retrieved_acceleration = registry.get_component<Acceleration>(entity).value();
  REQUIRE(retrieved_acceleration.x == 20);
  REQUIRE(retrieved_acceleration.y == 70);
  Velocity retrieved_velocity = registry.get_component<Velocity>(entity).value();
  REQUIRE(retrieved_velocity.x == 10);
  REQUIRE(retrieved_velocity.y == 20);

  Acceleration retrieved_acceleration2 = registry.get_component<Acceleration>(entity2).value();
  REQUIRE(retrieved_acceleration2.x == 80);
  REQUIRE(retrieved_acceleration2.y == 90);
  Velocity retrieved_velocity2 = registry.get_component<Velocity>(entity2).value();
  REQUIRE(retrieved_velocity2.x == 50);
  REQUIRE(retrieved_velocity2.y == 10);
}

TEST_CASE("Find component with multiple registered archetypes", "[heterogenes_archetype_retrieval]") {

  WorldRegistry registry { 10 };
  ComponentId id_vel = registry.register_component<Velocity>();
  ComponentId id_acc = registry.register_component<Acceleration>();

  EntityId entity { registry.create_entity<Acceleration, Velocity>() };
  EntityId entity2 { registry.create_entity<Velocity>() };

  registry.attach_component<Velocity>(entity, (Velocity){10, 20});
  registry.attach_component<Acceleration>(entity, (Acceleration){20, 70});
  registry.attach_component<Velocity>(entity2, (Velocity){50, 10});

  Acceleration retrieved_acceleration = registry.get_component<Acceleration>(entity).value();
  REQUIRE(retrieved_acceleration.x == 20);
  REQUIRE(retrieved_acceleration.y == 70);
  Velocity retrieved_velocity = registry.get_component<Velocity>(entity).value();
  REQUIRE(retrieved_velocity.x == 10);
  REQUIRE(retrieved_velocity.y == 20);

  Velocity retrieved_velocity2 = registry.get_component<Velocity>(entity2).value();
  REQUIRE(retrieved_velocity2.x == 50);
  REQUIRE(retrieved_velocity2.y == 10);
}
