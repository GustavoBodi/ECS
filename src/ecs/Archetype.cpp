#include "Archetype.hpp"

ArchetypeId Archetype::get_id()
{
  return id;
}

std::map<ComponentId, std::shared_ptr<ArchetypeEdge>> &Archetype::get_edges()
{
  return edges;
}

ArchetypeSignature Archetype::get_type()
{
  return type;
}

Column& Archetype::operator[](std::size_t index)
{
  if (index >= components.size()) {
    throw std::exception();
  }
  return components[index];
}

Archetype::~Archetype() {}
