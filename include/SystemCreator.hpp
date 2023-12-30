#pragma once
#include <memory>
#include "System.hpp"
#include "Types.hpp"

class SystemCreator {
public:
  /*! @brief Default constructor */
  SystemCreator(std::unordered_map<ComponentId, std::shared_ptr<ArchetypeMap>>
                    &component_archetype_mapping,
                IdController &id_controller,
                std::unordered_map<ArchetypeId, archetype_t> &archetype_index)
      : component_archetype_mapping{component_archetype_mapping},
        archetype_index{archetype_index},
        ids{id_controller} {};

  /*! @brief Auxiliary function for creating a new system */
  template <typename ...Components, typename Func>
  SystemBase *create_system(SystemId system_id, Func function, uint64_t tick_rate=1) {
    auto system = new System<Components...>(system_id, function, tick_rate, component_archetype_mapping, ids);
    std::vector<ComponentId> signature = ids.make_archetype_signature<Components...>();
    for (ComponentId component: signature) {
      auto archetype_map = component_archetype_mapping[component];
      if (archetype_map == nullptr)
        continue;
      for (auto it: *archetype_map) {
        ArchetypeId archetype_id = it.first;
        archetype_t archetype = archetype_index[archetype_id];
        if (archetype == nullptr)
          continue;
        auto signature_arch = archetype->get_type();
        auto signature_copy = signature;
        std::sort(signature_arch.begin(), signature_arch.end());
        std::sort(signature_copy.begin(), signature_copy.end());
        if (std::includes(signature_arch.begin(), signature_arch.end(), signature_copy.begin(), signature_copy.end()))
          system->add_archetype(archetype);
      }
    }
    return system;
  }

  /*! @brief Default destructor */
  ~SystemCreator() {};
private:
  /*! @brief Relation for the component and related archetypes */
  std::unordered_map<ComponentId, std::shared_ptr<ArchetypeMap>> &component_archetype_mapping;
  /*! @brief Relationship between a list of components and the archetypes */
  std::unordered_map<ArchetypeId, archetype_t> &archetype_index;
  /*! @brief Class responsible for creating the ids and managing their relations */
  IdController &ids;
};

