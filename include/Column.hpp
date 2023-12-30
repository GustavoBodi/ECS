#pragma once
#include <cstdint>
#include <memory>
#include <cstring>
#include "Types.hpp"

/*!
 * @brief Database Columns that represent components (vertically), the row is a representation
 * of an entity
 */
class Column {
  public:
    Column(std::size_t element_size, ComponentId type) : element_size{ element_size }, type{ type }
     {};

    /*!
     * @brief Overload of the indexing operator for selecting a line (Archetype)
     * @param index Return the value from the buffer of components
     */
    template <typename T>
    T get(std::size_t index) {
      T (*array)[max_amount] = (T(*)[max_amount]) elements.get();
      return (*array)[index];
    }

    template <typename T>
    T *get_vector() {
      T (*array)[max_amount] = (T(*)[max_amount]) elements.get();
      return *array;
    }

    std::size_t size() {
      return count;
    }

    /*!
     * @brief Overload of the indexing operator for selecting a line (Archetype)
     * @param index Return the value from the buffer of components
     */
    void *get(std::size_t index) {
      return &elements.get()[index * element_size];
    }

    /*!
     * @brief Inserts value in the column
     * @param component The component to be inserted
     * @tparam Component The component type
     */
    template <typename Component>
    std::size_t insert(Component component, std::size_t index) {
      if ( count == max_amount )
        throw std::exception();
      Component (*array)[max_amount] = (Component(*)[max_amount]) (elements.get());
      (*array)[index] = component;
      count++;
      return count - 1;
    }

    std::size_t insert(void *component, std::size_t index) {
      memcpy(&elements.get()[index * element_size], component, element_size);
      count++;
      return count - 1;
    }

    /*!
     * @brief Removes value from the column
     * @param index The index of the component to be deleted
     */
    void delete_component(std::size_t index) {
      if (index == count + 1)
        count --;
    }

  private:
    /*! @brief Size of an element */
    std::size_t element_size;
    /*! @brief Number of elements */
    std::size_t count { 0 };
    /*! @brief Max amount of elements in the array */
    static constexpr std::size_t max_amount { 100000 };
    /*! @brief Buffer with the components */
    std::unique_ptr<uint8_t[]> elements {new uint8_t [max_amount * element_size]};
    /*! @brief the type of component in the column */
    ComponentId type;
};
