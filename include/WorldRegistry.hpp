#pragma once
#include <optional>
#include <unordered_map>
#include "IdController.hpp"
#include "Archetype.hpp"
#include "Hasher.hpp"
#include "System.hpp"
#include "SystemCreator.hpp"
#include "TypeMapper.hpp"
#include "Types.hpp"


/*! @brief The registry for a world of entities, a program may have more than one */
class WorldRegistry {
public:
  /*! @brief Contructor to the world registry, will create its own archetype graph */
  WorldRegistry(uint64_t cycle_reset = 10);
  
  /*! @brief Destructor for the WorldRegistry */
  ~WorldRegistry() = default;

  /*! @brief Registers a new component in the registry */
  template <typename Component>
  ComponentId register_component();
  
  /*!
   * @brief Registers a system in the registry
   * @param system Lambda function that represents a system
   * @param tick_rate Every something ticks a system will run
   * when the registry itself ticks
   */
  template <typename ...T, typename Func>
  const SystemId register_system(Func system);

  /*!
   * @brief Disables a system tempararily
   * @param system The id of the system to be disabled
   */
  void disable_system(const SystemId system);

  /*!
   * @brief Return the component from the Registry
   * @param entity The of the entity to be searched
   * @param component The id of the component to be searched
   */
  template <typename T>
  std::optional<T> get_component(EntityId entity);

  /*!
   * @brief Return the component from the Registry
   * @param entity The of the entity to be searched
   * @param component The id of the component to be searched
   */
  void *get_component(EntityId entity, ComponentId component);

  /*!
   * @brief Creates a new entity
   * @param T Passes the initial achetype of the entity
   */
  template<typename ...Components>
  EntityId create_entity();

  /*!
   * @brief Adds a new Archetype
   * @param T passes the components of the new archetype
   */
  template<typename ...T>
  std::optional<ArchetypeId> register_archetype();

  /*!
   * @brief Adds a new Archetype
   * @param T passes the components of the new archetype
   */
  archetype_t register_archetype(std::vector<ComponentId> &components);

  /*!
   * @brief Adds Archetype to an entity
   * @param entity The entity the archetype will be added
   * @param archetype Reference to the archetype of the new entity
   */
  template <typename ...T>
  void add_archetype(EntityId entity);

  /*!
   * @brief Deletes and entity
   * @param entity The id of the entity
   */
  void delete_entity(EntityId entity);

  /*!
   * @brief Attaches a component to an entity that already fullfills the prerequisites
   * @param entity The entity id
   */
  template <typename T>
  void attach_component(EntityId entity, T component);

  /*! @brief Attaches a component to an entity with a void pointer */
  void attach_component(EntityId entity, ComponentId component_id , void *component);

  /*!
   * @brief Adds a certain component to an entity
   * @param entity EntityId that will have a component added
   * @param component Component to be added
   */
  template<typename T>
  archetype_t add_component(EntityId entity);

  /*!
   * @brief Removes a component from an entity
   * @param entity EntityId that will have its component removed
   * @param component The representation of the component to be removed
   */
  template<typename T>
  archetype_t remove_component(EntityId entity);

  /*! @brief Get archetype dependency graph */
  void get_dependency_graph(std::vector<ComponentId> &input) {
    std::vector<Archetype*> visited;
    list_each(root, input, visited);
  }

  /*! @brief Recursive function for showing each of the graph dependencies */
  void list_each(archetype_t archetype, std::vector<ComponentId> &input, std::vector<Archetype*> &visited);

  /*! @brief Ticks the entire registry */
  void tick();

  /*! @brief Returns the amount of registered components */
  std::size_t count_components();

  /*! @brief Returns the next id for a new entity */
  uint64_t get_id() { return ids.gen_entity_id(); }

  /*! @brief Return the id associated with the achetype type */
  template <typename ...T>
  ArchetypeId get_archetype_id();

  /*! @brief Returns the id associated with the component type */
  template <typename Component>
  ComponentId get_component_id() { return ids.get_component_id<Component>(); }

private:
  /*! @brief Creates a new mapping from the component to the archetypes */
  void create_component_archetype_mapping(archetype_t archetype);

  /*! @brief Creates the columns for all the components of the archetype */
  void create_archetype_columns(archetype_t archetype);

  /*! @brief Adds a node to the graph */
  archetype_t add_node(archetype_t archetype);

  /*! @brief Removes node from the graph */
  void remove_node(archetype_t archetype);

  /*! @brief Adds a node to the graph */
  archetype_t add_node(std::vector<ComponentId> type);

  /*! @brief Relation between and entity id with an archetype and a line */
  std::unordered_map<EntityId, std::shared_ptr<Record>> entity_index;
  /* @brief Auxiliary class for system creation */
  SystemCreator sys { component_archetype_mapping, ids, archetype_index };
  /*! @brief The class that generates the new ids, exists for composition purposes */
  IdController ids { };
  /*!
   * @brief Relation of a component Id with a relation of an archetype for the column
   * in the database
   */
  /*! @brief Mapping from each components and its associated archetypes */
  std::unordered_map<ComponentId, std::shared_ptr<ArchetypeMap>> component_archetype_mapping;
  /*! @brief Maps a component id with its size */
  std::unordered_map<ComponentId, std::size_t> component_size_index;
  /*! @brief Relation between a system id and a system */
  std::map<SystemId, SystemBase*> system_index;
  /*! @brief List of disabled systems */
  std::unordered_set<SystemId> disabled_systems_index;
  /*! @brief Relationship between a list of components and the archetypes */
  std::unordered_map<ArchetypeId, archetype_t> archetype_index;
  /*! @brief All the registered components and their depth in the graph */
  std::unordered_map<depth_t, std::tuple<archetype_t, dependencies_t>> depth_index;
  /*! @brief An archetype id list */
  TypeMapper<ArchetypeId> archetype_ids;
  /*! @brief Root archetype of the graph */
  archetype_t root { new Archetype {0, std::vector<ComponentId>()} };
  /*! @brief The controller for the graph operations */
  //GraphController graph {root};
  /*!
   * @brief Represents the internal abstractions of cycles
   * (relates mainly to the systems that should run each tick)
   */
  uint64_t cycle_reset;
  /*! @brief The cycle the world is currently in */
  uint64_t cycle;
};

template <typename Component>
ComponentId WorldRegistry::register_component() {
  ids.insert_component_id<Component>();
  component_size_index[ids.get_component_id<Component>()] = sizeof(Component);
  return ids.get_component_id<Component>();
}

template <typename ...Components>
EntityId WorldRegistry::create_entity() {
  EntityId new_entity = ids.gen_entity_id();
  if (!archetype_ids.contains<Components...>()) {
    register_archetype<Components...>();
  }
  ArchetypeId archetype_id { archetype_ids.find<Components...>()->second };
  archetype_t archetype { archetype_index[archetype_id] };
  std::shared_ptr<Record> entity_record { new Record {} };
  entity_record->archetype = archetype;
  entity_record->row = archetype->assign_row();
  entity_index[new_entity] = entity_record;
  return new_entity;
}

template <typename ...T>
std::optional<ArchetypeId> WorldRegistry::register_archetype() {
  ArchetypeId arch_id = ids.gen_archetype_id();
  archetype_ids.put<T...>(arch_id);
  archetype_t new_archetype { new Archetype(arch_id, ids.make_archetype_signature<T...>()) };
  archetype_index[arch_id] = new_archetype;
  add_node(new_archetype);
  create_component_archetype_mapping(new_archetype);
  create_archetype_columns(new_archetype);
  return std::make_optional(arch_id);
}

template <typename ...T, typename Func>
const SystemId WorldRegistry::register_system(Func system) {
  std::function<void(T*...)> wrapped_function { system };
  SystemBase *sys_class = sys.create_system<T...>(ids.gen_system_id(), wrapped_function);
  if (sys_class == nullptr)
    throw std::exception();
  system_index[sys_class->get_id()] = sys_class;
  return sys_class->get_id();
}

template <typename ...T>
ArchetypeId WorldRegistry::get_archetype_id() {
  return archetype_ids.find<T...>()->second;
}

template <typename ...T>
void WorldRegistry::add_archetype(EntityId entity) {
  ArchetypeId arch_id { archetype_ids.find<T...>()->second };
  archetype_t archetype { archetype_index[arch_id] };
}

template <typename T>
std::optional<T> WorldRegistry::get_component(EntityId entity) {
  if (entity_index[entity] == nullptr) {
    return std::nullopt;
  }
  std::shared_ptr<Record> record { entity_index[entity] };
  archetype_t archetype { record->archetype };
  ComponentId component_id = ids.get_component_id<T>();
  std::shared_ptr<ArchetypeMap> archetype_map { component_archetype_mapping[component_id] };
  if (archetype_map->count(archetype->get_id()) == 0) {
    return std::nullopt;
  }
  ArchetypeRecord a_record { (*archetype_map)[archetype->get_id()] };
  return std::make_optional((*archetype)[a_record].get<T>(record->row));
}

template <typename T>
void WorldRegistry::attach_component(EntityId entity, T component) {
  std::shared_ptr<Record> record { entity_index[entity] };
  archetype_t archetype { record->archetype };
  ComponentId component_id = ids.get_component_id<T>();
  std::shared_ptr<ArchetypeMap> archetype_map { component_archetype_mapping[component_id] };
  ArchetypeRecord a_record { (*archetype_map)[archetype->get_id()] };
  (*archetype)[a_record].insert(component, record->row);
}

template <typename T>
archetype_t WorldRegistry::add_component(EntityId entity) {
  std::shared_ptr<Record> record = entity_index[entity];
  archetype_t &current_archetype = record->archetype;
  std::vector<ComponentId> new_signature = current_archetype->get_type();
  std::vector<ComponentId> old_signature = current_archetype->get_type();
  ComponentId component = ids.get_component_id<T>();
  new_signature.push_back(component);
  archetype_t new_archetype;
  auto indexed_archetype = current_archetype->get_edges()[component];
  if (indexed_archetype == nullptr) {
    new_archetype = register_archetype(new_signature);
  } else {
    new_archetype = std::get<archetype_t>(indexed_archetype->add);
  }
  std::vector<void*> components;
  for (ComponentId component: old_signature) {
    components.push_back(get_component(entity, component));
  }
  current_archetype->remove_dependent_type();
  new_archetype->insert_dependent_type();
  record->archetype = new_archetype;
  record->row = new_archetype->assign_row();
  int i = 0;
  for (ComponentId component: old_signature) {
    attach_component(entity, component, components[i]);
    ++i;
  }
  return new_archetype;
}

template<typename T>
archetype_t WorldRegistry::remove_component(EntityId entity) {
  std::shared_ptr<Record> record = entity_index[entity];
  archetype_t &current_archetype = record->archetype;
  std::vector<ComponentId> new_signature = current_archetype->get_type();
  std::vector<ComponentId> old_signature = current_archetype->get_type();
  ComponentId component = ids.get_component_id<T>();
  auto comp_it = std::find(new_signature.begin(), new_signature.end(), component);
  new_signature.erase(comp_it);
  archetype_t new_archetype;
  auto indexed_archetype = current_archetype->get_edges()[component];
  if (indexed_archetype == nullptr) {
    new_archetype = register_archetype(new_signature);
  } else {
    new_archetype = std::get<archetype_t>(indexed_archetype->remove);
  }
  std::vector<void*> components;
  for (ComponentId component: new_signature) {
    components.push_back(get_component(entity, component));
  }
  current_archetype->remove_dependent_type();
  new_archetype->insert_dependent_type();
  record->archetype = new_archetype;
  record->row = new_archetype->assign_row();
  int i = 0;
  for (ComponentId component: new_signature) {
    attach_component(entity, component, components[i]);
    ++i;
  }
  return new_archetype;
}

