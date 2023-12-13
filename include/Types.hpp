#pragma once
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <unordered_map>

// Representações de ids
using ComponentId = uint64_t;
using ArchetypeId = uint64_t;
using EntityId = uint64_t;

// Lista de componentes de um arquétipo
using ArchetypeSignature = std::unordered_set<ComponentId>;

// Set of all the archetypes
using ArchetypeSet = std::unordered_set<ArchetypeId>;

// Para um certo arquétipo e um certo componente, essa entrada descreve a coluna
using ArchetypeRecord = std::size_t;

// Finding components in archetypes in O(1)
using ArchetypeMap = std::unordered_map<ArchetypeId, ArchetypeRecord>;

