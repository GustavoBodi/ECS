#include "WorldRegistry.hpp"
#include "Archetype.hpp"
#include "Types.hpp"
#include <algorithm>

WorldRegistry::WorldRegistry(uint64_t cycle_reset)
    : entity_index(), system_index(), archetype_index(), cycle_reset{cycle_reset}
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

void WorldRegistry::disable_system(const SystemId system) {
  disabled_systems_index.insert(system);
}

void WorldRegistry::tick() {
  for (auto system: system_index) {
    system.second->run();
    if (cycle == cycle_reset)
      cycle = 0;
    ++cycle;
  }
}

std::size_t WorldRegistry::count_components() {
  return ids.get_component_amount();
}

archetype_t WorldRegistry::add_node(archetype_t archetype) {
  return add_node(archetype->get_type());
}

archetype_t WorldRegistry::add_node(std::vector<ComponentId> type) {
  std::vector<ComponentId> &signature = type;
  std::vector<ComponentId> new_signature;
  archetype_t last = root;
  ComponentId last_component = signature[0];
  archetype_t it = root;
  std::size_t depth = 0;
  for (ComponentId component: signature) {
    new_signature.push_back(component);
    std::map<ComponentId, std::shared_ptr<ArchetypeEdge>> &edges = it->get_edges();
    if (!edges.contains(component)) {
      std::shared_ptr<ArchetypeEdge> new_edge ( new ArchetypeEdge() );
      std::tuple<ComponentId, std::size_t> component_depth = std::make_tuple(component, depth);
      std::tuple<archetype_t, std::size_t> indexed_tuple = depth_index[component_depth];
      if (std::get<archetype_t>(indexed_tuple) == nullptr) {
        archetype_t new_arch (new Archetype(ids.gen_archetype_id(), new_signature));
        depth_index[component_depth] = std::make_tuple(new_arch, 1);
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
  return it;
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

void WorldRegistry::create_archetype_columns(archetype_t archetype) {
  for (ComponentId component: archetype->get_type()) {
    archetype->create_column(component_size_index[component], component);
  }
}

void WorldRegistry::create_component_archetype_mapping(archetype_t archetype) {
  for (ComponentId component_id: archetype->get_type()) {
    ArchetypeRecord new_record { archetype->column_value(component_id)};
    if (component_archetype_mapping[component_id] == nullptr) {
      std::shared_ptr<ArchetypeMap> archetype_mapping { new ArchetypeMap {} };
      (*archetype_mapping)[archetype->get_id()] = new_record;
      component_archetype_mapping[component_id] = archetype_mapping;
    } else {
      std::shared_ptr<ArchetypeMap> old_map = component_archetype_mapping[component_id];
      (*old_map)[archetype->get_id()] = new_record;
    }
  }
}

void WorldRegistry::attach_component(EntityId entity, ComponentId component_id, void *component) {
  std::shared_ptr<Record> record { entity_index[entity] };
  if (record == nullptr) {
    return;
  }
  archetype_t archetype { record->archetype };
  std::shared_ptr<ArchetypeMap> archetype_map { component_archetype_mapping[component_id] };
  if (archetype_map == nullptr) {
    return;
  }
  ArchetypeRecord a_record { (*archetype_map)[archetype->get_id()] };
  (*archetype)[a_record].insert(component, record->row);
}

void *WorldRegistry::get_component(EntityId entity, ComponentId component_id) {
  if (entity_index[entity] == nullptr) {
    return nullptr;
  }
  std::shared_ptr<Record> record { entity_index[entity] };
  archetype_t archetype { record->archetype };
  std::shared_ptr<ArchetypeMap> archetype_map { component_archetype_mapping[component_id] };
  if (archetype_map->count(archetype->get_id()) == 0) {
    return nullptr;
  }
  ArchetypeRecord a_record { (*archetype_map)[archetype->get_id()] };
  return (*archetype)[a_record].get(record->row);
}

archetype_t WorldRegistry::register_archetype(std::vector<ComponentId> &components) {
  ArchetypeId arch_id = ids.gen_archetype_id();
  //archetype_ids.put<T...>(arch_id);
  archetype_t new_archetype { new Archetype(arch_id, components) };
  archetype_index[arch_id] = new_archetype;
  add_node(new_archetype);
  create_component_archetype_mapping(new_archetype);
  create_archetype_columns(new_archetype);
  return new_archetype;
}
