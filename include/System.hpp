#include <cstdint>

/*!
 * @brief Abstract class that represents a system, should be extended to represent a system
 */
class System {
public:
  /*!
   * @brief Register all the components of interest for this system
   */
  template <typename ...T>
  constexpr void register_components(T... list);
  /*!
   * @brief Runs the system
   */
  virtual void run() = 0;
  virtual ~System() {};
  /*!
   * @brief Return the tick rate of the system
   */
  uint64_t get_tick();

  /*!
   * @brief Deletes copy constructor to avoid misuse
   */
  System(const System &) = delete;

  /*!
   * @brief Deletes move constructor to avoid misuse
   */
  System(System &&) = delete;

  /*!
   * @brief Deletes copy constructor to avoid misuse
   */
  virtual System &operator=(const System &) = delete;

  /*!
   * @brief Deletes move constructor to avoid misuse
   */
  virtual System &operator=(System &&) = delete;
private:
  uint64_t every_x_tick;
};
