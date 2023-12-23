#pragma once
#include <optional>
#include <unordered_map>
#include "Archetype.hpp"
#include "System.hpp"
#include "TypeMapper.hpp"
#include "Types.hpp"

/*!
 * @brief System id representation
 */
using SystemId = uint64_t;

/*!
 * @brief The registry for a world of entities, a program may have more than one
 */
class WorldRegistry {
  public:
    /*!
     * @brief Contructor to the world registry, will create its own archetype graph
     */
    WorldRegistry(uint64_t cycle_reset);
    
    /*!
     * @brief Destructor for the WorldRegistry
     */
    ~WorldRegistry() {
      delete root;
    }

    /*!
     * @brief Registers a new component in the registry
     */
    template <typename T>
    ComponentId register_component();
    
    /*!
     * @brief Registers a system in the registry
     * @param system Lambda function that represents a system
     * @param tick_rate Every something ticks a system will run
     * when the registry itself ticks
     */
    template <typename Func, std::enable_if_t<std::is_function_v<Func>>, typename ...T>
    const SystemId register_system(Func system);

    /*!
     * @brief Disables a system tempararily
     * @param system The id of the system to be disabled
     */
    void disable_system(const SystemId system);

    /*!
     * @brief Creates an archetype signature from template parameters
     * @tparam Components The components that represent the archetype
     */
    template <typename ...Components>
    std::vector<ComponentId> make_archetype_signature();

    /*!
     * @brief Return the component from the Registry
     * @param entity The of the entity to be searched
     * @param component The id of the component to be searched
     */
    template <typename T>
    std::optional<T> get_component(EntityId entity);

    /*!
     * @brief Creates a new entity
     * @param T Passes the initial components of the entity
     */
    template<typename ...T>
    EntityId create_entity();

    /*!
     * @brief Adds a new Archetype
     * @param T passes the components of the new archetype
     */
    template<typename ...T>
    std::optional<ArchetypeId> register_archetype();

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

    /*!
     * @brief Adds a certain component to an entity
     * @param entity EntityId that will have a component added
     * @param component Component to be added
     */
    std::optional<const Archetype*> add_component(EntityId entity, ComponentId component);

    /*!
     * @brief Removes a component from an entity
     * @param entity EntityId that will have its component removed
     * @param component The representation of the component to be removed
     */
    std::optional<const Archetype*> remove_component(EntityId entity, ComponentId component);

    /*!
     * @brief Ticks the entire registry
     */
    void tick();

    /*l!
     * @brief Returns the amount of registered components
     */
    std::size_t count_components();

    /*!
     * @brief Returns the next id for a new entity
     */
    uint64_t get_id();

    /*!
     * @brief Returns the id associated with the component type
     */
    template <typename T>
    ComponentId get_component_id();

    /*!
     * @brief Return the id associated with the achetype type
     */
    template <typename ...T>
    ArchetypeId get_archetype_id();
  
    /*!
     * @brief Creates a new mapping from the component to the archetypes
     */
    template <typename ...T>
    void create_component_archetype_mapping();

    /*!
     * @brief Creates the columns for all the components of the archetype
     */
    template <typename ...T>
    void create_archetype_columns(Archetype *archetype);

  private:
    /*!
     * @brief Relation between and entity id with an archetype and a line
     */
    std::unordered_map<EntityId, Record*> entity_index;

    /*!
     * @brief Relation of a component Id with a relation of an archetype for the column
     * in the database
     */
    TypeMapper<ArchetypeMap*> component_archetype_mapping;

    /*!
     * @brief Maps a component with its size
     */
    TypeMapper<std::size_t> component_index;

    /*!
     * @brief Relation between a system id and a system
     */
    std::unordered_map<SystemId, System*> system_index;

    /*!
     * @brief List of disabled systems
     */
    std::unordered_set<SystemId> disabled_systems_index;

    /*!
     * @brief Relationship between a list of components and the archetypes
     */
    std::unordered_map<ArchetypeId, Archetype*> archetype_index;

    /*
     * @brief An archetype id list
     */
    TypeMapper<ArchetypeId> archetype_ids;

    /*!
     * @brief Root archetype of the graph
     */
    Archetype *root = new Archetype {0, std::vector<ComponentId>()};

    /*!
     * @brief Next entity id
     */
    uint64_t next_id;

    /*!
     * @brief Represents the internal abstractions of cycles (relates mainly to the systems that should run each tick)
     */
    uint64_t cycle_reset;

    /*!
     * @brief The cycle the world is currently in
     */
    uint64_t cycle;
};

template <typename T>
ComponentId WorldRegistry::get_component_id() {
  ComponentId id = component_index.id<T>();
}

template <typename T>
ComponentId WorldRegistry::register_component() {
  ComponentId id = component_index.put<T>(sizeof(T));
  return id;
}

template <typename ...T>
EntityId WorldRegistry::create_entity() {
  Archetype *root = root;
  EntityId new_entity = next_id++;
  if (!archetype_ids.contains<T...>()) {
    register_archetype<T...>();
  }
  ArchetypeId archetype_id = archetype_ids.find<T...>()->first;
  Archetype *archetype = archetype_index[archetype_id];
  Record *entity_record = new Record {};
  entity_record->archetype = archetype;
  auto row = archetype->assign_row();
  entity_record->row = row;
  entity_index[new_entity] = entity_record;
  return new_entity;
}

template <typename ...T>
std::optional<ArchetypeId> WorldRegistry::register_archetype() {
  ArchetypeId arch_id = archetype_ids.put<T...>(next_id++);
  Archetype *new_archetype = new Archetype(arch_id, make_archetype_signature<T...>());
  archetype_index[arch_id] = new_archetype;
  create_component_archetype_mapping<T...>();
  create_archetype_columns<T...>(new_archetype);
  return std::make_optional(arch_id);
}

template <typename Func, std::enable_if_t<std::is_function_v<Func>>, typename ...T>
const SystemId WorldRegistry::register_system(Func system) {
  SystemId id = next_id++;
}

template <typename ...T>
ArchetypeId WorldRegistry::get_archetype_id() {
  return archetype_ids.id<T...>();
}

template <typename ...T>
void WorldRegistry::add_archetype(EntityId entity) {
  ArchetypeId arch_id = archetype_ids.find<T...>()->first;
  Archetype *archetype = archetype_index[arch_id];
}

template <typename T>
std::optional<T> WorldRegistry::get_component(EntityId entity) {
  Record *record = entity_index[entity];
  Archetype *archetype = record->archetype;
  ArchetypeMap *archetype_map = component_archetype_mapping.find<T>()->second;
  if (archetype_map->count(archetype->get_id()) == 0) {
    return std::nullopt;
  }
  ArchetypeRecord a_record = (*archetype_map)[archetype->get_id()];
  return std::make_optional(static_cast<T>((*archetype)[a_record].get<T>(record->row)));
}

template <typename T>
void WorldRegistry::attach_component(EntityId entity, T component) {
  Record *record = entity_index[entity];
  Archetype *archetype = record->archetype;
  ArchetypeMap *archetype_map = component_archetype_mapping.find<T>()->second;
  ArchetypeRecord a_record = (*archetype_map)[archetype->get_id()];
  (*archetype)[a_record].insert(component, record->row);
}

template <typename ...Components>
std::vector<ComponentId> WorldRegistry::make_archetype_signature() {
  std::vector<ComponentId> archetype_signature {};
  ([&] {
          archetype_signature.push_back(component_index.id<Components>());
       } (), ...);
  return archetype_signature;
}

template <typename ...T>
void WorldRegistry::create_component_archetype_mapping() {
  ArchetypeId archetype_id = archetype_ids.id<T...>();
  Archetype* archetype = archetype_index[archetype_id];
  ([&] {
          ArchetypeMap *archetype_mapping = new ArchetypeMap {};
          ComponentId component_id = component_index.id<T>();
          (*archetype_mapping)[archetype_id] = ArchetypeRecord{ archetype->column_value(component_id) };
          component_archetype_mapping.put<T>(archetype_mapping);
       } (), ...);
}

template <typename ...T>
void WorldRegistry::create_archetype_columns(Archetype *archetype) {

  ([&] {
           archetype->create_column(sizeof(T), component_index.id<T>());
       } (), ...);
}
