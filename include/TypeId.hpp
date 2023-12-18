#include <atomic>

extern std::atomic_uint64_t TypeIdCounter;

template <typename T>
int getTypeId() {
  static uint64_t id = ++TypeIdCounter;
  return id;
}
