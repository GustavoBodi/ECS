#pragma once
#include <cstdint>
#include <unordered_set>
#include <unordered_map>

// Id internal representation
using ComponentId = uint64_t;
using ArchetypeId = uint64_t;
using EntityId = uint64_t;

// List of an component from archetypes
using ArchetypeSignature = std::unordered_set<ComponentId>;

// Set of all the archetypes
using ArchetypeSet = std::unordered_set<ArchetypeId>;

// For a certain archetype and a certain component, the entry describes the column
using ArchetypeRecord = std::size_t;

// Finding components in archetypes in O(1)
using ArchetypeMap = std::unordered_map<ArchetypeId, ArchetypeRecord>;

