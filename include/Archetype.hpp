#pragma once
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
  /*!
   * @brief Overload of the indexing operator for selecting a line (Archetype)
   * @param index Return the value from the buffer of components
   */
  void* operator[](std::size_t index) {
    return static_cast<void*>(static_cast<char*>(elements) + index * element_size);
  }

  private:
    void *elements;           // Buffer with the components
    std::size_t element_size; // Size of an element
    std::size_t count;        // Number of elements
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
     * @brief Archetype Destructor
     */
    ~Archetype();
  private:
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

