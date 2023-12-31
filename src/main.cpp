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

int main() {
  WorldRegistry registry { 10 };
  registry.register_component<Velocity>();
  registry.register_component<Speed>();
  auto speed_system = [] (Velocity &v, Speed &s) {
    s.x += v.x;
    s.y += v.y;
  };
  //registry.register_system<Velocity, Speed>(speed_system);
  for (int i = 0; i < 100000; ++i) {
    EntityId entity = registry.create_entity<Velocity, Speed>();
    registry.attach_component(entity, (Velocity){10 * i, 2 * i});
    registry.attach_component(entity, (Speed){5 * i, 3 * i, i});
  }
  return 0;
}
