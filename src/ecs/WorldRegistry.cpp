#include "WorldRegistry.hpp"
#include "Archetype.hpp"
#include "Types.hpp"

WorldRegistry::WorldRegistry(uint64_t cycle_reset)
    : entity_index(), component_index(), system_index(), archetype_index(),
      next_id{0}, cycle_reset{cycle_reset}
{}

std::optional<void*> WorldRegistry::get_component(EntityId entity, ComponentId component) {
  Record &record = entity_index[entity];
  Archetype *archetype = record.archetype;
  ArchetypeMap archetype_map = component_archetype_mapping[component];
  if (archetype_map.count(archetype->get_id()) == 0) {
    return std::nullopt;
  }
  ArchetypeRecord &a_record = archetype_map[archetype->get_id()];
  return std::make_optional((*archetype)[a_record][record.row]);
}

void WorldRegistry::delete_entity(EntityId entity) {}

std::optional<const Archetype*> WorldRegistry::add_component(EntityId entity, ComponentId component) {
  // Find the record
  Record &record = entity_index[entity];
  ArchetypeSignature &signature = record.archetype->get_type();
  ArchetypeSignature copy = signature;
  copy.insert(component);

  // If there is no archetype, archetype insert vertex in the graph

  // Add Archetype to component list of archetypes

  // Move entity to the right of the graph

  // Insert into the entity index
}

std::optional<const Archetype*> WorldRegistry::remove_component(EntityId entity, ComponentId component) {}

void WorldRegistry::disable_system(const SystemId system) {
  disabled_systems_index.insert(system);
}

void WorldRegistry::tick() {
  for (auto system: system_index) {
    bool is_disabled = disabled_systems_index.count(system.first) != 0;
    bool should_run = system.second.get_tick() % cycle == 0;
    if (!is_disabled && should_run)
        system.second.run();
    if (cycle == cycle_reset)
      cycle = 0;
    ++cycle;
  }
}

std::size_t WorldRegistry::count_components() {
  return component_index.size();
}

uint64_t WorldRegistry::get_id() {
  return next_id;
}

