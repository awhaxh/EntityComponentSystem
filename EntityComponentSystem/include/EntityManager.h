/**
 * @file EntityManager.h
 * @author Ashley Low
 * @brief Implements an Entity-Component-System (ECS) manager and generic component storage.
 * @date September 13, 2025
 *
 * This file defines:
 * - IComponentStorage: Base interface for component storage.
 * - ComponentStorage<C>: Template for storing components of type C.
 * - EntityManager: Handles creation/destruction of entities and managing their components.
 *
 * Features:
 * - Efficient entity ID recycling.
 * - Type-safe component storage using templates.
 * - forEach iteration for systems (mutable or read-only).
 * - Supports any number of component types via std::type_index.
 */
#pragma once

using Entity = int;

// ------------------------------------------------------------------
// Base interface for all component storage
// ------------------------------------------------------------------
class IComponentStorage {
public:
    virtual ~IComponentStorage() = default;

    /// Remove component associated with an entity
    virtual void remove(Entity e) = 0;

    /// Check if an entity has this component
    virtual bool has(Entity e) const = 0;
};

// ------------------------------------------------------------------
// Template storage for components of type C
// ------------------------------------------------------------------
template<typename C>
class ComponentStorage : public IComponentStorage {
public:
    ComponentStorage() = default;
    ~ComponentStorage() override = default;

    /**
     * @brief Adds or updates a component for an entity.
     * @tparam Args Constructor argument types for the component.
     * @param e Entity ID.
     * @param args Arguments forwarded to component constructor.
     * @return Reference to the stored component.
     */
    template<typename... Args>
    C& emplace(Entity e, Args&&... args) {
        auto [it, inserted] = data.emplace(e, C{ std::forward<Args>(args)... });
        if (!inserted)
            it->second = C{ std::forward<Args>(args)... };
        return it->second;
    }

    /**
     * @brief Remove a component associated with an entity.
     * @param e Entity ID.
     */
    void remove(Entity e) override {
        data.erase(e);
    }

    /**
     * @brief Check if an entity has this component.
     * @param e Entity ID.
     * @return True if entity has this component, false otherwise.
     */
    bool has(Entity e) const override {
        return data.find(e) != data.end();
    }

    /**
     * @brief Mutable access to a component.
     * @param e Entity ID.
     * @return Pointer to the component or nullptr if not found.
     */
    C* get(Entity e) {
        auto it = data.find(e);
        return (it != data.end()) ? &it->second : nullptr;
    }

    /**
     * @brief Const access to a component.
     * @param e Entity ID.
     * @return Pointer to the component or nullptr if not found.
     */
    const C* get(Entity e) const {
        auto it = data.find(e);
        return (it != data.end()) ? &it->second : nullptr;
    }

    /**
     * @brief Access all components (mutable).
     * @return Reference to the internal map of components.
     */
    std::unordered_map<Entity, C>& all() { return data; }

    /**
     * @brief Access all components (const).
     * @return Const reference to the internal map of components.
     */
    const std::unordered_map<Entity, C>& all() const { return data; }

private:
    std::unordered_map<Entity, C> data; 
};

// ------------------------------------------------------------------
// Entity manager
// ------------------------------------------------------------------
class EntityManager {
public:
    EntityManager() : nextId(0) {}

    /**
     * @brief Create a new entity ID.
     * @return New entity ID.
     */
    Entity createEntity() {
        Entity id;
        if (!freeIds.empty()) {
            id = freeIds.back();
            freeIds.pop_back();
        }
        else {
            id = nextId++;
        }
        entities.push_back(id);
        return id;
    }

    /**
     * @brief Destroy an entity and remove all its components.
     * @param id Entity ID to destroy.
     */
    void destroyEntity(Entity id) {
        entities.erase(std::remove(entities.begin(), entities.end(), id), entities.end());
        freeIds.push_back(id);

        for (auto& kv : components) {
            kv.second->remove(id);
        }
    }

    /**
     * @brief Add or update a component for an entity.
     * @tparam T Component type.
     * @param e Entity ID.
     * @param args Arguments forwarded to component constructor.
     * @return Reference to the stored component.
     */
    template<typename T, typename... Args>
    T& addComponent(Entity e, Args&&... args) {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if (it == components.end()) {
            auto store = std::make_unique<ComponentStorage<T>>();
            ComponentStorage<T>* ptr = store.get();
            components.emplace(ti, std::move(store));
            return ptr->emplace(e, std::forward<Args>(args)...);
        }
        else {
            auto* ptr = static_cast<ComponentStorage<T>*>(it->second.get());
            return ptr->emplace(e, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Remove a component of type T from an entity.
     * @tparam T Component type.
     * @param e Entity ID.
     */
    template<typename T>
    void removeComponent(Entity e) {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if (it == components.end()) return;
        auto* ptr = static_cast<ComponentStorage<T>*>(it->second.get());
        ptr->remove(e);
    }

    /**
     * @brief Check if an entity has a component of type T.
     * @tparam T Component type.
     * @param e Entity ID.
     * @return True if the entity has this component, false otherwise.
     */
    template<typename T>
    bool hasComponent(Entity e) const {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if (it == components.end()) return false;
        auto* ptr = static_cast<ComponentStorage<T>*>(it->second.get());
        return ptr->has(e);
    }

    /**
     * @brief Get a mutable pointer to a component of type T.
     * @tparam T Component type.
     * @param e Entity ID.
     * @return Pointer to the component or nullptr if not found.
     */
    template<typename T>
    T* getComponent(Entity e) {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if (it == components.end()) return nullptr;
        auto* ptr = static_cast<ComponentStorage<T>*>(it->second.get());
        return ptr->get(e);
    }

    /**
     * @brief Get a const pointer to a component of type T.
     * @tparam T Component type.
     * @param e Entity ID.
     * @return Const pointer to the component or nullptr if not found.
     */
    template<typename T>
    const T* getComponent(Entity e) const {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if (it == components.end()) return nullptr;
        auto* ptr = static_cast<ComponentStorage<T>*>(it->second.get());
        return ptr->get(e);
    }

    /**
     * @brief Iterate over all entities with a component T (mutable)
     * @param func Callable that accepts (Entity, T&)
     */
    template<typename T, typename Func>
    void forEach(Func func) {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if (it == components.end()) return;
        auto* ptr = static_cast<ComponentStorage<T>*>(it->second.get());
        for (auto& kv : ptr->all()) {
            func(kv.first, kv.second);
        }
    }

    /**
     * @brief Iterate over all entities with a component T (const)
     * @param func Callable that accepts (Entity, const T&)
     */
    template<typename T, typename Func>
    void forEach(Func func) const {
        std::type_index ti(typeid(T));
        auto it = components.find(ti);
        if (it == components.end()) return;
        auto* ptr = static_cast<const ComponentStorage<T>*>(it->second.get());
        for (const auto& kv : ptr->all()) {
            func(kv.first, kv.second);
        }
    }

private:
    int nextId; 
    std::vector<Entity> entities; 
    std::vector<Entity> freeIds; 
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> components; 
};
