#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Forward declaration
struct Archetype;
struct ArchetypeEdge;
struct Record;
struct ArchetypeRecord;

// Representações de ids
using ComponentId = uint64_t;
using ArchetypeId = uint64_t;
using EntityId = uint64_t;

// Lista de componentes de um arquétipo
using Type = std::vector<ComponentId>;

// Colunas do banco de dados Componentes (Verticalmente)
class Column {
  public:
  // Overload do operador para poder selecionar a linha (Arquetipo)
  void* operator[](std::size_t index) {
    return static_cast<void*>(static_cast<char*>(elements) + index * element_size);
  }

  private:
    void *elements; // Buffer com os Components
    std::size_t element_size; // Tamanho de um elemento
    std::size_t count; // Número de elementos
};

struct ArchetypeEdge {
  Archetype &add;
  Archetype &remove;
};

// Estrutura de um Arquétipo linha do banco de dados
// arquetipo e seu id
struct Archetype {
  ArchetypeId id;
  Type type;
  std::vector<Column> components;
  std::unordered_map<ComponentId, ArchetypeEdge> edges;
};

struct Record {
  Archetype & archetype;
  std::size_t row;
};

// Para um certo arquétipo e um certo componente, essa entrada descreve a coluna
struct ArchetypeRecord {
  std::size_t column;
};

using ArchetypeSet = std::unordered_set<ArchetypeId>;

// Finding components in archetypes in O(1)
using ArchetypeMap = std::unordered_map<ArchetypeId, ArchetypeRecord>;


