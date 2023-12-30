#pragma once
#include <cstdint>
#include <iostream>
#include <functional>
#include <variant>
#include "Archetype.hpp"
#include "IdController.hpp"
#include "Types.hpp"

/*! @brief System base class, represents a pointer to an instance of a system */
class SystemBase {
public:
  /*! @brief Default constructor */
  SystemBase(SystemId system_id, uint64_t tick,
             std::unordered_map<ComponentId, std::shared_ptr<ArchetypeMap>>
                 &component_archetype_mapping, IdController &id_controller, std::vector<ComponentId> signature)
      : id{system_id}, every_x_tick{tick},
        component_archetype_mapping{component_archetype_mapping}, ids{id_controller}, signature{signature} {};

  /*! @brief Runs the system */
  virtual void run() = 0;

  /*! @brief Returns the stored system id */
  SystemId get_id() { return id; }

  /*! @brief Return the tick rate of the system */
  uint64_t get_tick() { return every_x_tick; }

  /*! @brief Adds archetype to the system */
  void add_archetype(archetype_t archetype) {
    auto signature_arch = archetype->get_type();
    std::sort(signature_arch.begin(), signature_arch.end());
    std::sort(signature.begin(), signature.end());
    if (std::includes(signature_arch.begin(), signature_arch.end(), signature.begin(), signature.end())) {
      archetype_list.insert(archetype); 
    }
  }

  /* @brief Removes archetype from the system */
  void remove_archetype(archetype_t archetype) {
    auto find_it = std::find(archetype_list.begin(), archetype_list.end(), archetype);
    if (find_it != archetype_list.end())
      archetype_list.erase(find_it);
  }

  /*! @brief Deletes copy constructor to avoid misuse */
  SystemBase(const SystemBase &) = delete;

  /*! @brief Deletes move constructor to avoid misuse */
  SystemBase(SystemBase &&) = delete;

  /*! @brief Deletes copy constructor to avoid misuse */
  virtual SystemBase &operator=(const SystemBase &) = delete;

  /*! @brief Deletes move constructor to avoid misuse */
  virtual SystemBase &operator=(SystemBase &&) = delete;

  /*! @brief Default destructor */
  virtual ~SystemBase() {};

protected:
  /*! @brief System global id */
  SystemId id;
  /*! @brief Represent how many ticks of interval for running this system again */
  uint64_t every_x_tick;
  /* @brief List of observed archetypes by the system */
  std::unordered_set<archetype_t> archetype_list {};
  /* @brief Reference to the mapping used to find the component columns in archetypes */
  std::unordered_map<ComponentId, std::shared_ptr<ArchetypeMap>> &component_archetype_mapping;
  /* @brief The id controller (used for getting the component ids) */
  IdController &ids;
  /* @brief Internal system signature */
  std::vector<ComponentId> signature;
};

/*! @brief Abstract class that represents a system, should be extended to represent an atomic operation */
template <typename ...Components>
class System : public SystemBase {
public:
  /*! @brief Default constructor */
  System(SystemId system_id, std::function<void(Components*...)> function,
         uint64_t tick,
         std::unordered_map<ComponentId, std::shared_ptr<ArchetypeMap>>
             &component_archetype_mapping,
         IdController &id_controller, std::vector<ComponentId> signature)
      : SystemBase{system_id, tick, component_archetype_mapping, id_controller, signature},
        system_fn{function} {};

  /*! @brief Default destructor */
  ~System() {};

  /*! @brief Runs an instance of system */
  void run() {
    for (archetype_t archetype: archetype_list) {
      std::array<std::pair<void*, std::size_t>, sizeof...(Components)> dependency_list;
      int i = 0;
      ([&] {
              std::shared_ptr<ArchetypeMap> archetype_map { component_archetype_mapping[ids.get_component_id<Components>()] };
              ArchetypeRecord a_record { (*archetype_map)[archetype->get_id()] };
              if (archetype_map->count(archetype->get_id()) == 0) {
                return;
              }
              try {
                dependency_list[i] = (*archetype)[a_record].get_vector<Components>();
              } catch (std::exception e) {
                return;
              }
              ++i;
              } (), ...);
      std::size_t iter = dependency_list[0].second;
      std::tuple<Components*...> input;
      for (int i = 0; i < iter; ++i) {
        auto temp_fn = system_fn;
        int j = 0;
        ([&] {
          auto part_input =  &(((Components*) dependency_list[j].first)[i]);
          std::get<Components*>(input) = part_input;
          ++j;
        } (), ...);
        std::apply(system_fn, input);
      }
    }
  }

private:
  /*! @brief Function that represents the system inner working */
  std::function<void(Components*...)> system_fn;
};
