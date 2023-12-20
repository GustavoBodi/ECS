#include <atomic>

extern std::atomic_uint64_t TypeIdCounter;

/*!
 * @brief Auxiliary function for returning an increasing id each template instantiation,
 * return the same value for the same type
 */
template <typename T>
int getTypeId() {
  static uint64_t id = ++TypeIdCounter;
  return id;
}
