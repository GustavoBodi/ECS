#pragma once
#include <optional>
#include <unordered_map>
#include "Archetype.hpp"
#include "Hasher.hpp"
#include "System.hpp"
#include "TypeMapper.hpp"

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
    template <typename Func, typename=std::enable_if_t<std::is_function_v<Func>>, typename ...T>
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
    std::optional<void*> get_component(EntityId entity, ComponentId component);

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
    void add_archetype(EntityId entity, ArchetypeId &archetype);

    /*!
     * @brief Deletes and entity
     * @param entity The id of the entity
     */
    void delete_entity(EntityId entity);

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

    /*!
     * @brief Returns the amount of registered components
     */
    std::size_t count_components();

    /*!
     * @brief Returns the next id for a new entity
     */
    uint64_t get_id();

  private:
    /*!
     * @brief Relation between and entity id with an archetype and a line
     */
    std::unordered_map<EntityId, Record> entity_index;

    /*!
     * @brief Relation of a component Id with a relation of an archetype for the column
     * in the database
     */
    std::unordered_map<ComponentId, ArchetypeMap> component_archetype_mapping;

    /*!
     * @brief Maps a component with its size
     */
    std::unordered_map<ComponentId, std::size_t> component_index;

    /*!
     * @brief Relation between a system id and a system
     */
    std::unordered_map<SystemId, System&> system_index;

    /*!
     * @brief List of disabled systems
     */
    std::unordered_set<SystemId> disabled_systems_index;

    /*!
     * @brief Relationship between a list of components and the archetypes
     */
    std::unordered_map<ArchetypeSignature, Archetype, Hasher<ComponentId>> archetype_index;

    /*!
     * @brief Root archetype of the graph
     */
    Archetype *root = new Archetype {0};

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
ComponentId WorldRegistry::register_component() {
  component_index[next_id++] = sizeof(T);
  return next_id - 1;
}

template <typename ...T>
EntityId WorldRegistry::create_entity()
{
  Archetype *root = root;
  EntityId new_entity = next_id++;
  return new_entity;
}

template <typename ...T>
std::optional<ArchetypeId> WorldRegistry::register_archetype() {
  Archetype new_archetype {++next_id};
}

