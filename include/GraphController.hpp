#pragma once
#include "Archetype.hpp"

struct Edge {
  ComponentId component;
  Archetype *archetype;
};

class GraphController {
  public:
    GraphController(archetype_t root): root {root} {};

    ~GraphController() = default;

    std::size_t len();

    void remove_node(archetype_t archetype, ArchetypeId archetype_id);


  private:
    std::unordered_set<ArchetypeId> all_archetypes;
    archetype_t root;
};
