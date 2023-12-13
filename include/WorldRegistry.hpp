#pragma once

#include <optional>
#include <unordered_map>
#include "Archetype.hpp"
#include "Hasher.hpp"
#include "System.hpp"

using SystemId = uint64_t;

/*
 * @brief The registry for a world of entities, a program may have more than one
 */
class WorldRegistry {
  public:
    /*
     * @brief Contructor to the world registry, will create its own archetype graph
     */
    WorldRegistry(Archetype &root);

    /*
     * @brief Return the component from the Registry
     * @param entity The of the entity to be searched
     * @param component The id of the component to be searched
     */
    std::optional<void*> get_component(EntityId entity, ComponentId component);

    /*
     * @brief Creates a new entity
     * @param component_list Passes the initial components of the entity
     */
    EntityId create_entity(ArchetypeSignature component_list);

    /*
     * @brief Deletes and entity
     * @param entity The id of the entity
     */
    void delete_entity(EntityId entity);

    /*
     * @brief Adds a certain component to an entity
     * @param entity EntityId that will have a component added
     * @param component Component to be added
     */
    std::optional<const Archetype*> add_component(EntityId entity, ComponentId component);

    /*
     * @brief Removes a component from an entity
     * @param entity EntityId that will have its component removed
     * @param component The representation of the component to be removed
     */
    std::optional<const Archetype*> remove_component(EntityId entity, ComponentId component);

  private:
    /*
     * @brief Relaciona o Id de entidade com um arquétipo e uma linha
     */
    std::unordered_map<EntityId, Record> entity_index;
    /*
     * @brief Relaciona o Id de componente com uma relação de arquétipo para coluna no
     * banco de dados
     */
    std::unordered_map<ComponentId, ArchetypeMap> component_index;
    /*
     * @brief Relaciona um id de sistema com o sistema
     */
    std::unordered_map<SystemId, System&> system_index;
    /*
     * @brief Relação entre uma lista de componentes e um arquétipo
     */
    std::unordered_map<ArchetypeSignature, Archetype, Hasher<ComponentId>> archetype_index;
    /*
     * @brief Arquétipo raiz do grafo
     */
    Archetype &root;
    /*
     * @brief Próximo id de entidade
     */
    EntityId next_id;
};

