#include <unordered_map>
#include <atomic>

template <typename Type>
class TypeMapper {
  private:
    typedef std::unordered_map<uint64_t, Type> MapperType;

  public:
    /*!
     * @brief Type alias for the iterator
     */
    using iterator = typename MapperType::iterator;

    /*!
     * @brief Type alias for the const iterator
     */
    using const_iterator = typename MapperType::const_iterator;

    /*!
     * @brief Type alias for the value
     */
    using value_type = typename MapperType::value_type;

    /*!
     * @brief Const begin iterator
     */
    const_iterator begin() const { return mapper.begin(); }

    /*!
     * @brief Const end iterator
     */
    const_iterator end() const { return mapper.end(); }

    /*!
     * @brief Begin iterator
     */
    iterator begin() { return mapper.begin(); }

    /*!
     * @brief End iterator
     */
    iterator end() { return mapper.end(); }

    /*!
     * @brief Returns the size
     */
    std::size_t size() { return _size; }

    /*!
     * @brief Returns the size as const
     */
    std::size_t size() const { return _size; }

    /*!
     * @brief Return true if the map contains the type
     */
    template <typename ...Key>
    bool contains() const { return mapper.contains(get_type_id<Key...>()); }

    /*!
     * @brief Finds a mapping based on the type
     */
    template <typename ...Key>
    iterator find() { return mapper.find(get_type_id<Key...>()); }

    /*!
     * @brief Finds a mapping (const) based on the type
     */
    template <typename ...Key>
    const_iterator find() const { return mapper.find(get_type_id<Key...>()); }

    /*!
     * @brief Removes based on a type mapping
     */
    template <typename ...Key>
    Type remove() {
      auto value = find<Key...>()->second;
      mapper.erase(get_type_id<Key...>());
      return value;
    }

    /*!
     * @brief Inserts in the type mapping, move semantics
     */
    //template <typename ...Key>
    //uint64_t put(Type &&value) {
    //  uint64_t id { get_type_id<Key...>() };
    //  mapper[id] = std::forward<Type>(value);
    //  ++_size;
    //  return id;
    //}

    /*!
     * @brief Inserts in the type mapping, copy (for pointers)
     */
    template <typename ...Key>
    uint64_t put(Type value) {
      uint64_t id { get_type_id<Key...>() };
      mapper[id] = std::forward<Type>(value);
      ++_size;
      return id;
    }

    /*!
     * @brief Returns the id (internal) from the type
     */
    template <typename ...Key>
    uint64_t id() {
      return get_type_id<Key...>();
    }

  private:
    std::size_t _size { 0 };
    MapperType mapper;
    template <typename ...Key>
    inline static uint64_t get_type_id() {
      static const uint64_t id = LastTypeId++;
      return id;
    }

    static std::atomic_uint64_t LastTypeId;
};

template <class ValueType>
std::atomic_uint64_t TypeMapper<ValueType>::LastTypeId(0);
