#include "Archetype.hpp"

struct Edge {
  ComponentId component;
  Archetype *archetype;
};

class GraphController {
  public:
    GraphController(Archetype *root);
    ~GraphController();
    std::size_t len();
    void add_edge();
    void remove_edge();
    void add_vertex();
    void remove_vertex();

    template <typename ...T>
    void list_each();

  private:
    Archetype *root;
};
