#pragma once
#include <exception>
#include <vector>
#include <unordered_map>
#include "Types.hpp"

/*!
 * @brief Forward declaration of the archetypes graph
 */
struct ArchetypeEdge;

/*!
 * @brief Database Columns that represent components (vertically), the row is a representation
 * of an entity
 */
class Column { // Equivalent to an ecs_type_t
  public:
    Column(std::size_t element_size, ComponentId type) : element_size{ element_size }, type{ type } {
      elements = static_cast<void*>(new char[max_amount * element_size]);
    };
    ~Column() {
      delete [] static_cast<char*>(elements);
    }
    /*!
     * @brief Overload of the indexing operator for selecting a line (Archetype)
     * @param index Return the value from the buffer of components
     */
    template <typename T>
    void* get(std::size_t index) {
      T (*array)[max_amount] = (T(*)[max_amount]) elements;
      return (*array)[index];
    }

    /*!
     * @brief Inserts value in the column
     * @param component The component to be inserted
     */
    template <typename T>
    std::size_t insert(T component) {
      if ( count == max_amount )
        throw std::exception();
      T (*array)[max_amount] = (T(*)[max_amount]) elements;
      (*array)[count] = component;
      count++;
      return count - 1;
    }

  private:
    void *elements;           // Buffer with the components
    std::size_t element_size; // Size of an element
    std::size_t count { 0 };        // Number of elements
    std::size_t max_amount { 1000 };
    ComponentId type;
};

/*!
 * @brief Structure of an Archetype in a line of the database
 * with the data and its id
 */
class Archetype {
  public:
    /*!
     * @brief The archetype constructor, gets its id from the registry
     * @param id The id of the new archetype
     */
    Archetype(ArchetypeId id): id{id} {};

    /*!
     * @brief Returns the id from the archetype
     */
    ArchetypeId get_id();

    /*!
     * @brief Returns a reference to the edges of the archetype
     */
    std::unordered_map<ComponentId, ArchetypeEdge>& get_edges();

    /*!
     * @brief Returns the archetype signature
     */
    ArchetypeSignature &get_type();

    /*!
     * @brief Overloads the indexing operator for getting the components
     */
    Column &operator[] (std::size_t index);

    /*!
     * @brief Returns the current amount of registered intems
     */
    std::size_t size() { return _size; }

    std::size_t assign_row() { _size++; return _size; };

    /*!
     * @brief Archetype Destructor
     */
    ~Archetype();
  private:
    std::size_t _size { 0 };
    ArchetypeId id;
    ArchetypeSignature type;
    std::vector<Column> components;
    std::unordered_map<ComponentId, ArchetypeEdge> edges;
};

/*!
 * @brief Connections to other Archetypes
 */
struct ArchetypeEdge {
  Archetype *add;
  Archetype *remove;
};

/*!
 * @brief The Record is an archetype relation with its row on the database
 */
struct Record {
  Archetype *archetype;
  std::size_t row;
};

