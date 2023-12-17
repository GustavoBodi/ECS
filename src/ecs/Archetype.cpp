#include "Archetype.hpp"

Archetype::Archetype(ArchetypeId id): id {id}
{
}

ArchetypeId Archetype::get_id()
{
}

std::unordered_map<ComponentId, ArchetypeEdge>& Archetype::get_edges()
{
}

ArchetypeSignature& Archetype::get_type()
{
}

Column& Archetype::operator[](std::size_t index)
{
}

