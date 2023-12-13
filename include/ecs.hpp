#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
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

class TypeHasher {
  public:
    std::size_t operator()(Type const& vec) const {
      std::size_t seed = vec.size();
      for(auto x : vec) {
        x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
        x = x ^ (x >> 31);
        seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
      return seed;
    }
};


// Colunas do banco de dados Componentes (Verticalmente)
class Column {
  public:
  // Overload do operador para poder selecionar a linha (Arquetipo)
  void* operator[](std::size_t index) {
    return static_cast<void*>(static_cast<char*>(elements) + index * element_size);
  }

  private:
    void *elements; // Buffer com of Components
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

using SystemId = uint64_t;

class System {
};

class WorldRegistry {
  public:
    WorldRegistry(Archetype &root)
        : entity_index(), component_index(), system_index(), archetype_index(),
          root{root}, next_id{0} {}
    std::optional<void*> get_component(EntityId entity, ComponentId component) {
      Record &record = entity_index[entity];
      Archetype &archetype = record.archetype;
      ArchetypeMap archetype_map = component_index[component];

      if (archetype_map.count(archetype.id) == 0) {
        return std::nullopt;
      }

      ArchetypeRecord &a_record = archetype_map[archetype.id];
      return std::make_optional(archetype.components[a_record.column][record.row]);
    }

  private:
    // Relaciona o Id de entidade com um arquétipo e uma linha
    std::unordered_map<EntityId, Record> entity_index;
    // Relaciona o Id de componente com uma relação de arquétipo para coluna no
    // banco de dados
    std::unordered_map<ComponentId, ArchetypeMap> component_index;
    // Relaciona um id de sistema com o sistema
    std::unordered_map<SystemId, System> system_index;
    // Relação entre uma lista de componentes e um arquétipo
    std::unordered_map<Type, Archetype, TypeHasher> archetype_index;
    // Arquétipo raiz do grafo
    Archetype &root;
    // Próximo id de entidade
    EntityId next_id;
};

