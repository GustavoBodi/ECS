#include "WorldRegistry.hpp"
#include "Archetype.hpp"
#include "Types.hpp"
#include <algorithm>

WorldRegistry::WorldRegistry(uint64_t cycle_reset)
    : entity_index(), component_index(), system_index(), archetype_index(),
      next_id{0}, cycle_reset{cycle_reset}
{}

void WorldRegistry::delete_entity(EntityId entity) {
  std::shared_ptr<Record> record = entity_index[entity];
  archetype_t archetype { record->archetype };
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

void WorldRegistry::add_node(archetype_t archetype) {
  std::vector<ComponentId> signature = archetype->get_type();
  std::vector<ComponentId> new_signature;
  archetype_t last = root;
  ComponentId last_component = signature[0];
  archetype_t it = root;
  std::size_t depth = 0;
  for (ComponentId component: signature) {
    new_signature.push_back(component);
    std::unordered_map<ComponentId, std::shared_ptr<ArchetypeEdge>> &edges = it->get_edges();
    if (!edges.contains(component)) {
      std::shared_ptr<ArchetypeEdge> new_edge ( new ArchetypeEdge() );
      std::tuple<ComponentId, std::size_t> component_depth = std::make_tuple(component, depth);
      std::tuple<archetype_t, std::size_t> indexed_tuple = depth_index[component_depth];
      if (std::get<archetype_t>(indexed_tuple) == nullptr) {
        archetype_t new_arch (new Archetype(next_id, new_signature));
        depth_index[component_depth] = std::make_tuple(new_arch, 1);
        ++next_id;
        new_edge->add = std::make_tuple(new_arch, component);
      } else {
        auto new_arch = std::get<archetype_t>(indexed_tuple);
        new_edge->add = std::make_tuple(new_arch, component);
        auto new_entry = std::make_tuple(new_arch, std::get<std::size_t>(indexed_tuple) + 1);
        depth_index[component_depth] = new_entry;
      }
      new_edge->remove = std::make_tuple(last, last_component);
      edges[component] = new_edge;
    }
    last_component = component;
    last = it;
    it = std::get<archetype_t>(edges.find(component)->second->add);
    depth++;
  }
}

void WorldRegistry::list_each(archetype_t archetype, std::vector<ComponentId> &input,
    std::vector<Archetype*> &visited ) {
  auto edges = archetype->get_edges();
  for (auto edge: edges) {
    archetype_t next_archetype = std::get<archetype_t>(edge.second->add);
    ComponentId component_id = std::get<ComponentId>(edge.second->add);
    if (std::find(visited.begin(), visited.end(), next_archetype.get()) != visited.end()) {
      return;
    }
    input.push_back(component_id);
    visited.push_back(next_archetype.get());
    list_each(std::get<archetype_t>(edge.second->add), input, visited);
  }
}
