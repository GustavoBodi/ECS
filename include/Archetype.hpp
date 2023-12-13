#pragma once
#include <vector>
#include <unordered_map>
#include "Types.hpp"

struct ArchetypeEdge;

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


// Estrutura de um Arquétipo linha do banco de dados
// arquetipo e seu id
struct Archetype {
  ArchetypeId id;
  Type type;
  std::vector<Column> components;
  std::unordered_map<ComponentId, ArchetypeEdge> edges;
};

struct ArchetypeEdge {
  Archetype &add;
  Archetype &remove;
};

class Record {
  public:
    Archetype & archetype;
    std::size_t row;
};

