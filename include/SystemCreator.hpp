#pragma once
#include <memory>
#include "System.hpp"
#include "Types.hpp"

class SystemCreator {
public:
  /*! @brief Default constructor */
  SystemCreator(std::unordered_map<ComponentId, std::shared_ptr<ArchetypeMap>>
                    &component_archetype_mapping, IdController &id_controller)
      : component_archetype_mapping{component_archetype_mapping}, ids{id_controller} {};

  /*! @brief Auxiliary function for creating a new system */
  template <typename ...Components, typename Func>
  std::shared_ptr<SystemBase> create_system(SystemId system_id, Func function, uint64_t tick_rate=1) {
    std::function<void(Components&...)> wrapped_function { function };
    std::shared_ptr<System<Components...>> derived_system { new System<Components...>(system_id, wrapped_function, tick_rate, component_archetype_mapping, ids) };
    std::shared_ptr<SystemBase> new_system = derived_system;
    return new_system;
  }

  /*! @brief Default destructor */
  ~SystemCreator() {};
private:
  std::unordered_map<ComponentId, std::shared_ptr<ArchetypeMap>> &component_archetype_mapping;
  IdController &ids;
};

