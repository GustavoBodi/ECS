#pragma once
#include <iostream>
#include <memory>
#include "Archetype.hpp"

struct Edge {
  ComponentId component;
  Archetype *archetype;
};

class GraphController {
  public:
    GraphController(std::shared_ptr<Archetype> root): root {root} {};

    ~GraphController() = default;

    std::size_t len();

    void remove_node(std::shared_ptr<Archetype> archetype, ArchetypeId archetype_id);


  private:
    std::unordered_set<ArchetypeId> all_archetypes;
    std::shared_ptr<Archetype> root;
};
