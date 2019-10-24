#include "ECS.h"

void Entity::addGroup(GroupID group) {
    groupBitSet[group] = true;
    m_manager.AddToGroup(this, group);
}
