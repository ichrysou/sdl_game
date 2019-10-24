#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using GroupID = std::size_t;

inline ComponentID getNewComponentTypeID() {
    static ComponentID lastID = 0u;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

/* base class */
class Component {
   public:
    Entity* entity; /* reference to the owner */

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual ~Component() {}
};

class Entity {
   private:
    Manager& m_manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitSet;

   public:
    Entity(Manager& manager) : m_manager(manager) {}

    void update() {
        for (auto& c : components) c->update();
    }
    void draw() {
        for (auto& c : components) c->draw();
    }

    bool isActive() const { return active; }
    void destroy() { active = false; }

    template <typename T>
    bool hasComponent() const {
        return componentBitSet[getComponentTypeID<T>()];
    }
    bool hasGroup(GroupID group) const { return groupBitSet[group]; }

    void addGroup(GroupID group);
    void delGroup(GroupID group) { groupBitSet[group] = false; }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template <typename T>
    T& getComponent() const {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
};

class Manager {
   private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;

   public:
    void update() {
        for (auto& e : entities) e->update();
    }
    void draw() {
        for (auto& e : entities) e->draw();
    }
    void refresh() {
        for (auto i(0u); i < maxGroups; i++) {
            auto& v(groupedEntities[i]);
            v.erase(std::remove_if(std::begin(v), std::end(v), [i](Entity* entity) { return !entity->isActive() || !entity->hasGroup(i); }),
                    std::end(v));
        }

        /* remove_if re-orders the elements in a way that:
         * the elements in the vector that are too be removed go to the end
         * of the vector. Then returns the iterator at the beginning of the elements
         * to be removed. There, erase will take action and clear those  obsolete elements out */
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                                      [](const std::shared_ptr<Entity>& mEntity) { return !mEntity->isActive(); }),
                       std::end(entities));
    }

    void AddToGroup(Entity* lentity, GroupID lgroup) { groupedEntities[lgroup].emplace_back(lentity); }

    std::vector<Entity*>& getGroup(GroupID lgroup) { return groupedEntities[lgroup]; }
    // TODO: refactor this service to
    // have better performance (e.g. filtering in entt)
    template <typename T>
    std::vector<std::shared_ptr<Entity>> getEntities() {
        std::vector<std::shared_ptr<Entity>> filtered;
        for (auto& e : entities) {
            if (e->hasComponent<T>()) {
                filtered.emplace_back(e);
            }
        }
        return filtered;
    }

    Entity& addEntity() {
        Entity* e = new Entity(*this);
        std::shared_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};
