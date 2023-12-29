#include "Types.hpp"

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
  SystemId gen_system_id() { component++; return component; }

  /*! @brief Returns a new entity id */
  EntityId gen_entity_id() { entity++; return entity; }

  /*! @brief Returns a new archetype id */
  ArchetypeId gen_archetype_id() { archetype++; return archetype; }

private:
  /*! @brief The current entity id in the controller */
  EntityId entity { 0 };

  /*! @brief The current component id in the controller */
  ComponentId component { 0 };

  /*! @brief The current system id in the controller */
  SystemId system { 0 };

  /*! @brief The current archetype id in the controller */
  ArchetypeId archetype { 0 };
};
