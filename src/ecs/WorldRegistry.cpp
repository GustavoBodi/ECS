#include "../../include/WorldRegistry.hpp"
#include "../../include/Archetype.hpp"
#include "../../include/Types.hpp"

WorldRegistry::WorldRegistry()
    : entity_index(), component_index(), system_index(), archetype_index(),
      next_id{0} 
{}

std::optional<void*> WorldRegistry::get_component(EntityId entity, ComponentId component) {
  Record &record = entity_index[entity];
  Archetype *archetype = record.archetype;
  ArchetypeMap archetype_map = component_archetype_mapping[component];
  if (archetype_map.count(archetype->id) == 0) {
    return std::nullopt;
  }
  ArchetypeRecord &a_record = archetype_map[archetype->id];
  return std::make_optional(archetype->components[a_record][record.row]);
}

EntityId WorldRegistry::create_entity(ArchetypeSignature &component_list)
{
  EntityId new_entity = next_id++;
  return new_entity;
}

void WorldRegistry::delete_entity(EntityId entity) {}

std::optional<const Archetype*> WorldRegistry::add_component(EntityId entity, ComponentId component) {}

std::optional<const Archetype*> WorldRegistry::remove_component(EntityId entity, ComponentId component) {}

void WorldRegistry::disable_system(const SystemId system) {}

void WorldRegistry::tick() {
  for (auto system: system_index) {
    system.second.run();
  }
}

std::size_t WorldRegistry::count_components() {
  return component_index.size();
}

uint64_t WorldRegistry::get_id() {
  return next_id;
}
