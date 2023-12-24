#include "WorldRegistry.hpp"
#include "Archetype.hpp"
#include "Types.hpp"

WorldRegistry::WorldRegistry(uint64_t cycle_reset)
    : entity_index(), component_index(), system_index(), archetype_index(),
      next_id{0}, cycle_reset{cycle_reset}
{}

void WorldRegistry::delete_entity(EntityId entity) {
  std::shared_ptr<Record> record = entity_index[entity];
  std::shared_ptr<Archetype> archetype { record->archetype };
  if (archetype->size() == 0) {
    archetype_index.erase(archetype->get_id());
  }
  entity_index.erase(entity);
  std::size_t row = record->row;
  ArchetypeSignature signature = archetype->get_type();
  for (ComponentId component_id: signature) {
    std::shared_ptr<ArchetypeMap> archetype_map { component_archetype_mapping[component_id] };
    ArchetypeRecord a_record { (*archetype_map)[archetype->get_id()] };
    (*archetype)[a_record].delete_component(record->row);
  }

}

std::optional<const Archetype*> WorldRegistry::add_component(EntityId entity, ComponentId component) {
  // Find the record
  std::shared_ptr<Record> record = entity_index[entity];
  ArchetypeSignature &signature = record->archetype->get_type();
  ArchetypeSignature copy = signature;
  copy.push_back(component);

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
  for (auto &system: system_index) {
    bool is_disabled = disabled_systems_index.count(system.first) != 0;
    bool should_run = system.second->get_tick() % cycle == 0;
    if (!is_disabled && should_run)
        system.second->run();
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

