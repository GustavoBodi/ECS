#pragma once
#include "Archetype.hpp"
#include "Types.hpp"
#include <cstdint>
#include <functional>

/*! @brief System base class, represents a pointer to an instance of a system */
class SystemBase {
public:
  /*! @brief Default constructor */
  SystemBase(SystemId system_id, uint64_t tick): id{system_id}, every_x_tick{tick} {};

  /*! @brief Runs the system */
  virtual void run() = 0;

  /*! @brief Returns the stored system id */
  SystemId get_id() { return id; }

  /*! @brief Return the tick rate of the system */
  uint64_t get_tick() { return every_x_tick; }

  /*! @brief Adds archetype to the system */
  void add_archetype(archetype_t archetype) { archetype_list.push_back(archetype); }

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
  std::vector<archetype_t> archetype_list {};
};

/*! @brief Abstract class that represents a system, should be extended to represent an atomic operation */
template <typename ...Components>
class System : public SystemBase {
public:
  /*! @brief Default constructor */
  System(SystemId system_id, std::function<void(Components...)> function,
         uint64_t tick)
      : SystemBase{system_id, tick}, system_fn{function} {};

  /*! @brief Default destructor */
  ~System() {};

  void run() {
    for (archetype_t archetype: archetype_list) {
      //system_fn(params...);
    }
  }

private:
  /*! @brief Function that represents the system inner working */
  std::function<void(Components...)> system_fn;
};
