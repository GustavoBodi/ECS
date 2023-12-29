#include <cstdint>

/*! @brief Abstract class that represents a system, should be extended to represent an atomic operation */
class System {
public:
  /*! @brief Register all the components of interest for this system */
  template <typename ...T>
  constexpr void register_components(T... list);

  /*! @brief Default destructro */
  virtual ~System() = default;

  /*! @brief Runs the system */
  virtual void run() = 0;

  /*! @brief Return the tick rate of the system */
  uint64_t get_tick();

  /*! @brief Deletes copy constructor to avoid misuse */
  System(const System &) = delete;

  /*! @brief Deletes move constructor to avoid misuse */
  System(System &&) = delete;

  /*! @brief Deletes copy constructor to avoid misuse */
  virtual System &operator=(const System &) = delete;

  /*! @brief Deletes move constructor to avoid misuse */
  virtual System &operator=(System &&) = delete;
private:
  /*! @brief Represent how many ticks of interval for running this system again */
  uint64_t every_x_tick;
};
