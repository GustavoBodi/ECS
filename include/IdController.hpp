#pragma once
#include "Types.hpp"
#include "TypeMapper.hpp"

/*!
 * @brief The class that generates the ids for the program, exists for 
 * separation of concerns and composition purposes
 */
class IdController {
public:
  /*! @brief Default constructor */
  IdController() = default;

  /*! @brief Default destructor */
  ~IdController() = default;

  /*! @brief Returns a new component id */
  ComponentId gen_component_id() { component++; return component; }

  /*! @brief Returns a new system id */
  SystemId gen_system_id() { system++; return system; }

  /*! @brief Returns a new entity id */
  EntityId gen_entity_id() { entity++; return entity; }

  /*! @brief Returns a new archetype id */
  ArchetypeId gen_archetype_id() { archetype++; return archetype; }

  /*! @brief Returns the id associated with the component type */
  template <typename Component>
  ComponentId get_component_id() { return component_index.find<Component>()->second; }

  /*! @brief Inserts a component in the type mapper */
  template <typename Component>
  void insert_component_id() { component_index.put<Component>(gen_component_id()); }

  /*! @brief Returns the amount of components in the component mapper */
  std::size_t get_component_amount() { return component_index.size(); }

  /*!
   * @brief Creates an archetype signature from template parameters
   * @tparam Components The components that represent the archetype
   */
  template <typename ...Components>
  std::vector<ComponentId> make_archetype_signature() {
    std::vector<ComponentId> archetype_signature {};
    ([&] {
            archetype_signature.push_back(get_component_id<Components>());
         } (), ...);
    return archetype_signature;
  }
  
private:
  /*! @brief The current entity id in the controller */
  EntityId entity { 0 };
  /*! @brief The current component id in the controller */
  ComponentId component { 0 };
  /*! @brief The current system id in the controller */
  SystemId system { 0 };
  /*! @brief The current archetype id in the controller */
  ArchetypeId archetype { 0 };
  /*! @brief Maps a component type with its id */
  TypeMapper<ComponentId> component_index;
};
