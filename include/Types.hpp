#pragma once
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <unordered_map>

/*! @brief Component id internal representation */
using ComponentId = uint64_t;

/*! @brief Archetype id internal representation */
using ArchetypeId = uint64_t;

/*! @brief Entity id internal representation */
using EntityId = uint64_t;

/*! @brief System id representation */
using SystemId = uint64_t;

/*! @brief List of an component from archetypes */
using ArchetypeSignature = std::vector<ComponentId>;

/*! @brief Set of all the archetypes */
using ArchetypeSet = std::unordered_set<ArchetypeId>;

/*! @brief For a certain archetype and a certain component, the entry describes the column */
using ArchetypeRecord = std::size_t;

/*! @brief Finding components in archetypes in O(1) */
using ArchetypeMap = std::unordered_map<ArchetypeId, ArchetypeRecord>;

/*! @brief The amount of dependent types a node current relates to */
using dependencies_t = std::size_t;

/*! @brief The depth of a node in the graph */
using depth = std::size_t;

/*!
 * @brief Representation of a component depth in the graph
 * ComponentId, depth, dependencies
 */
using depth_t = std::tuple<ComponentId, depth>;
