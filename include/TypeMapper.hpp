#include "Types.hpp"
#include <unordered_map>
#include <atomic>

template <typename Type>
class TypeMapper {
  private:
    typedef std::unordered_map<ComponentId, Type> MapperType;

  public:
    typedef typename MapperType::iterator iterator;
    typedef typename MapperType::const_iterator const_iterator;
    typedef typename MapperType::value_type value_type;

    const_iterator begin() const { return mapper.begin(); }

    const_iterator end() const { return mapper.end(); }

    iterator begin() { return mapper.begin(); }

    iterator end() { return mapper.end(); }

    template <typename Key>
    iterator find() { return mapper.find(get_type_id<Key>()); }

    template <typename Key>
    const_iterator find() const { return mapper.find(get_type_id<Key>()); }

    template <typename Key>
    void put(Type &&value) {
      mapper[get_type_id<Key>()] = std::forward<Type>(value);
    }

  private:
    MapperType mapper;
    template <typename Key>
    inline static int get_type_id() {
      static const int id = LastTypeId++;
      return id;
    }

    static std::atomic_int LastTypeId;
};

template <class ValueType>
std::atomic_int TypeMapper<ValueType>::LastTypeId(0);
