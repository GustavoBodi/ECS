#pragma once
#include <exception>
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>
#include <memory>
#include "Types.hpp"
#include "Column.hpp"

/*! @brief Forward declaration of the archetypes graph */
struct ArchetypeEdge;

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
    Archetype(ArchetypeId id, std::vector<ComponentId> ids): id{id}, type{ids} {};

    /*! @brief Returns the id from the archetype */
    ArchetypeId get_id();

    /*! @brief Returns a reference to the edges of the archetype */
    std::map<ComponentId, std::shared_ptr<ArchetypeEdge>> &get_edges();

    /*! @brief Returns the archetype signature */
    ArchetypeSignature get_type();

    /*! @brief Overloads the indexing operator for getting the components */
    Column &operator[] (std::size_t index);

    /*! @brief Returns the current amount of registered items */
    std::size_t size() { return _size; }

    /*! @brief Returns true if there is not dependent type left */
    bool is_empty() {
      return dependent_type == 0;
    }

    /*! @brief Removes a dependent entity */
    void remove_dependent_type() {
      --dependent_type;
    }

    /*! @brief Inserts a new dependent type */
    void insert_dependent_type() {
      ++dependent_type;
    }

    /*! @brief Returns the row for a new entity */
    std::size_t assign_row() { 
      _size++;
      return _size; 
    };

    /*! @brief Returns the index of the Component in signature */
    const std::size_t column_value(ComponentId component) const {
      auto result = std::find(type.begin(), type.end(), component);
      if (result == type.end()) {
        throw std::exception();
      }
      auto index = std::distance(type.begin(), result);
      return index;
    }

    /*! @brief Creates a new Column with a certain type */
    void create_column(std::size_t element_size, ComponentId type) {
      components.push_back(Column(element_size, type));
    }

    /*! @brief Archetype Destructor */
    ~Archetype() = default;

  private:
    /*! @brief Current assigned row */
    std::size_t _size { 0 };
    /*! @brief Archetypes that depend on this archetype */
    std::size_t dependent_type { 0 };
    /*! @brief Id representation */
    ArchetypeId id;
    /*! @brief Component archetype representation */
    ArchetypeSignature type;
    /*! @brief Vector that stores the columns that represent the array of components */
    std::vector<Column> components;
    /*! @brief Graph edges for other archetypes */
    std::map<ComponentId, std::shared_ptr<ArchetypeEdge>> edges;
};

/*! @brief Connections to other Archetypes */
struct ArchetypeEdge {
  std::tuple<std::shared_ptr<Archetype>, ComponentId> add;
  std::tuple<std::shared_ptr<Archetype>, ComponentId> remove;
};

/*! @brief The Record is an archetype relation with its row on the database */
struct Record {
  std::shared_ptr<Archetype> archetype;
  std::size_t row;
};

/* @brief Alias for Archetype pointer */
using archetype_t = std::shared_ptr<Archetype>;
