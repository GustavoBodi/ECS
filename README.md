# Entity Component System (Archetype based)
## Overview
This is an entity component system based on archetype architecture in modern
C++. There are 3 fundamental parts of the system:

> Components: Data that exists for one concern.

> Entity: An Id that represents an entity in the Internal code structure

> System: Logic that operates over the components for some repetitive action

Entity component systems solve the problem of wide and deep inheritance that
exists mainly in game and UI programming.

## Internal Structure

### Entities 
Each entity in the system is represented by an Id. As this is an Archetype based
ECS, each Archetype will naturaly have each Id mapped to an array index. This relation
is represented by a record.

### Components
Each component in the system is represented by a database column. For indexing it in the array,
the archetype record is used.

### System
The system is represented by any entry valid for the metaprogramming function is_function_v().
So, any function lamba, std::function or overloaded operator() is valid entry for a system.

## Example

## License
This program is licensed under the MIT license.
