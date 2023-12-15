#pragma once
#include <vector>
#include <unordered_map>
#include "Types.hpp"

struct ArchetypeEdge;

/*
 * @brief Colunas do banco de dados que representa os Componentes (Verticalmente)
 */
class Column { // Equivalent to an ecs_type_t
  public:
  /*
   * @brief Overload of the indexing operator for selecting a line (Archetype)
   */
  void* operator[](std::size_t index) {
    return static_cast<void*>(static_cast<char*>(elements) + index * element_size);
  }

  private:
    void *elements;           // Buffer with the components
    std::size_t element_size; // Size of an element
    std::size_t count;        // Number of elements
};

/* 
 * @brief Structure of an Archetype in a line of the database
 * with the data and its id
 */
struct Archetype {
  ArchetypeId id;
  ArchetypeSignature type;
  std::vector<Column> components;
  std::unordered_map<ComponentId, ArchetypeEdge> edges;
};

/*
 * @brief Connections to other Archetypes
 */
struct ArchetypeEdge {
  Archetype *add;
  Archetype *remove;
};

/*
 * @brief The Record is an archetype relation with its row on the database
 */
struct Record {
  Archetype *archetype;
  std::size_t row;
};

