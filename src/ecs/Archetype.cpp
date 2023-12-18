#include "Archetype.hpp"

Archetype::Archetype(ArchetypeId id): id {id}
{
}

ArchetypeId Archetype::get_id()
{
  return id;
}

std::unordered_map<ComponentId, ArchetypeEdge>& Archetype::get_edges()
{
  return edges;
}

ArchetypeSignature& Archetype::get_type()
{
  return type;
}

Column& Archetype::operator[](std::size_t index)
{
  return components[index];
}

Archetype::~Archetype() {}
