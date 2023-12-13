#pragma once

#include <optional>
#include <unordered_map>
#include "Archetype.hpp"
#include "Hasher.hpp"

using SystemId = uint64_t;

/*
 * @brief The registry for a world of entities, a program may have more than one
 */
class WorldRegistry {
  public:
    /*
     * @brief Contructor to the world registry, will create its own archetype graph
     */
    WorldRegistry(Archetype &root)
        : entity_index(), component_index(), system_index(), archetype_index(),
          root{root}, next_id{0} {}

    /*
     * @brief Return the component from the Registry
     * @param entity The of the entity to be searched
     * @param component The id of the component to be searched
     */
    std::optional<void*> get_component(EntityId entity, ComponentId component) {
      Record &record = entity_index[entity];
      Archetype &archetype = record.archetype;
      ArchetypeMap archetype_map = component_index[component];

      if (archetype_map.count(archetype.id) == 0) {
        return std::nullopt;
      }

      ArchetypeRecord &a_record = archetype_map[archetype.id];
      return std::make_optional(archetype.components[a_record.column][record.row]);
    }

    /*
     * @brief Creates a new entity
     * @param component_list Passes the initial components of the entity
     */
    EntityId create_entity(Type component_list);

    /*
     * @brief Adds a certain component to an entity
     * @param entity EntityId that will have a component added
     * @param component Component to be added
     */
    std::optional<Archetype&> add_component(EntityId entity, ComponentId component);

    /*
     * @brief Removes a component from an entity
     * @param entity EntityId that will have its component removed
     * @param component The representation of the component to be removed
     */
    std::optional<Archetype&> remove_component(EntityId entity, ComponentId component);

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
    std::unordered_map<SystemId, System> system_index;
    /*
     * @brief Relação entre uma lista de componentes e um arquétipo
     */
    std::unordered_map<Type, Archetype, Hasher<ComponentId>> archetype_index;
    /*
     * @brief Arquétipo raiz do grafo
     */
    Archetype &root;
    /*
     * @brief Próximo id de entidade
     */
    EntityId next_id;
};

