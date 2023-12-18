#include <atomic>

extern std::atomic_int TypeIdCounter;

template <typename T>
int getTypeId() {
  static int id = ++TypeIdCounter;
  return id;
}
