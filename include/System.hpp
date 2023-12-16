#include <cstdint>

/*
 * @brief Abstract class that represents a system, should be extended to represent a system
 */
class System {
public:
  System(const System &) = delete;
  template <typename ...T>
  constexpr void register_components(T... list);
  virtual void run() = 0;
  virtual ~System();
  uint64_t get_tick();

  System(System &&) = delete;
  virtual System &operator=(const System &) = delete;
  virtual System &operator=(System &&) = delete;
private:
  uint64_t every_x_tick;
};
