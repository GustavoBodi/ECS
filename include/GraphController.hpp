#pragma once
#include "Archetype.hpp"
#include "IdController.hpp"

struct Edge {
  ComponentId component;
  Archetype *archetype;
};

class GraphController {
  public:
    GraphController(
        archetype_t root,
        IdController &ids)
        : root{root}, ids{ids} {};

    ~GraphController() = default;

    void remove_node(archetype_t archetype);

    archetype_t add_node(archetype_t archetype);

    archetype_t add_node(std::vector<ComponentId> type);

  private:
  archetype_t root;
  //std::unordered_map<depth_t, std::tuple<archetype_t, dependencies_t>> depth_index;
  IdController &ids;
};
