#pragma once
#include <deque>
#include <vector>

#include "glm/vec2.hpp"

namespace ukariga {
    inline Vector2 to_ray_vec(const glm::vec2& vec) {
        return Vector2{vec.x, vec.y};
    }

    inline glm::vec2 to_glm_vec(const Vector2& vec) {
        return glm::vec2{vec.x, vec.y};
    }

    class EntityBase {
    public:
        using TypeId = uint16_t;
        using UpdateEntitiesFunc = void (*)(float delta);
        using PhysicsUpdateEntitiesFunc = void (*)(float delta);
        using DrawEntitiesFunc = void (*)(float alpha);

        explicit EntityBase(const TypeId id) : id{id} {
        }

        [[nodiscard]] TypeId get_id() const {
            return id;
        }

        static void update_all(const float delta) {
            for (auto entity_func: update_funcs) {
                entity_func(delta);
            }
        }

        static void physics_update_all(const float delta) {
            for (auto entity_func: physics_update_funcs) {
                entity_func(delta);
            }
        }

        static void draw_all(const float alpha) {
            for (auto entity_func: draw_funcs) {
                entity_func(alpha);
            }
        }

        glm::vec2 position{};

    protected:
        template<typename>
        static TypeId get_id() {
            static TypeId id = get_next_type_id();
            return id;
        }

        template<typename>
        static void register_type(UpdateEntitiesFunc f1, PhysicsUpdateEntitiesFunc f2, DrawEntitiesFunc f3) {
            update_funcs.push_back(f1);
            physics_update_funcs.push_back(f2);
            draw_funcs.push_back(f3);
        }

        glm::vec2 previous_position{};

    private:
        static TypeId get_next_type_id() {
            static TypeId id_counter = 0;
            return id_counter++;
        }

        inline static std::vector<UpdateEntitiesFunc> update_funcs;
        inline static std::vector<PhysicsUpdateEntitiesFunc> physics_update_funcs;
        inline static std::vector<DrawEntitiesFunc> draw_funcs;
        TypeId id{};
    };

    namespace concepts {
        template<typename T>
        concept OverridesUpdate = requires(T t, float delta)
        {
            t.update_impl(delta);
        };

        template<typename T>
        concept OverridesPhysicsUpdate = requires(T t, float delta)
        {
            t.physics_update_impl(delta);
        };

        template<typename T>
        concept OverridesDraw = requires(T t, glm::vec2 draw_position)
        {
            t.draw(draw_position);
        };
    }

    template<typename Derived>
    class Entity : public EntityBase {
    public:
        template<typename... Args>
        static Derived& create(Args&&... args) {
            static auto register_type = [] {
                EntityBase::register_type<Derived>(update_entities, physics_update_entities, draw_entities);
                return 0;
            }();
            entities.emplace_back(std::forward<Args>(args)...);
            return entities.back();
        }

        static TypeId get_id() {
            return get_id<Derived>();
        }

        static void reserve(size_t size) {
            entities.reserve(size);
        }

        void update(float delta) {
            static_cast<Derived*>(this)->update_impl(delta);
        }

        void physics_update(float delta) {
            previous_position = position;
            static_cast<Derived*>(this)->physics_update_impl(delta);
        }

        [[nodiscard]] glm::vec2 get_draw_position(const float alpha) const {
            return previous_position + (position - previous_position) * alpha;
        }

        void draw(const float alpha) {
            static_cast<Derived*>(this)->draw_impl(get_draw_position(alpha));
        }

    protected:
        Entity() : EntityBase(EntityBase::get_id<Derived>()) {
        }

    private:
        static void update_entities(const float delta) {
            for (auto& entity: entities) {
                entity.update(delta);
            }
        }

        static void physics_update_entities(const float delta) {
            for (auto& entity: entities) {
                entity.physics_update(delta);
            }
        }

        static void draw_entities(const float alpha) {
            for (auto& entity: entities) {
                entity.draw(alpha);
            }
        }

        inline static std::deque<Derived> entities;
    };
}
