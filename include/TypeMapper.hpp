#include <unordered_map>
#include <atomic>

template <typename Type>
class TypeMapper {
  private:
    typedef std::unordered_map<uint64_t, Type> MapperType;

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
    uint64_t put(Type &&value) {
      uint64_t id { get_type_id<Key>() };
      mapper[id] = std::forward<Type>(value);
      return id;
    }

  private:
    MapperType mapper;
    template <typename Key>
    inline static uint64_t get_type_id() {
      static const uint64_t id = LastTypeId++;
      return id;
    }

    static std::atomic_uint64_t LastTypeId;
};

template <class ValueType>
std::atomic_uint64_t TypeMapper<ValueType>::LastTypeId(0);
