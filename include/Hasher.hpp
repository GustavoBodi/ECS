#pragma once
#include <tuple>
#include <type_traits>
#include <vector>
#include <boost/functional/hash.hpp>
#include <tuple>

/*! @brief Template class for hashing vectors of some kind of id */
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
class VectorHasher {
public:
  /*! @brief Overloaded function call for satisfying std::hash */
  std::size_t operator()(std::vector<T> const& vec) const {
    std::size_t seed = vec.size();
    for(auto x : vec) {
      x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
      x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
      x = x ^ (x >> 31);
      seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
  }
};

namespace std
{
/*! @brief Boost implementation of hashing for tuples */
template<typename... T>
struct hash<tuple<T...>>
{
    size_t operator()(tuple<T...> const& arg) const noexcept
    {
        return boost::hash_value(arg);
    }
};
}
