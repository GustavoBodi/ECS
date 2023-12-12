#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Representações de ids
using ComponentId = uint64_t;
using ArchetypeId = uint64_t;
using EntityId = uint64_t;

// Lista de componentes de um arquétipo
using Type = std::vector<ComponentId>;

struct Column {
  void *elements; // Buffer com of Components
  std::size_t element_size; // Tamanho de um elemento
  std::size_t count; // Número de elementos
  void* operator[](std::size_t index) {
    return static_cast<void*>(static_cast<char*>(elements) + index * element_size);
  }
};

// Estrutura de um Arquétipo
struct Archetype {
  ArchetypeId id;
  Type type;
  std::vector<Column> components;
};

std::unordered_map<Type, Archetype> archetype_index;

struct Record {
  Archetype & archetype;
  std::size_t row;
};

struct ArchetypeRecord {
  std::size_t column;
};

using ArchetypeMap = std::unordered_map<ArchetypeId, ArchetypeRecord>;

std::unordered_map<ComponentId, ArchetypeMap> component_index;

std::unordered_map<EntityId, Record> entity_index;

using ArchetypeSet = std::unordered_set<ArchetypeId>;

void *get_component(EntityId entity, ComponentId component) {
  Record &record = entity_index[entity];
  Archetype &archetype = record.archetype;

  ArchetypeMap archetype_map = component_index[component];
  if (archetype_map.count(archetype.id) == 0) {
    return nullptr;
  }

  ArchetypeRecord &a_record = archetype_map[archetype.id];
  return archetype.components[a_record.column][record.row];
}

