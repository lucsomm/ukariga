#pragma once
#include <vector>

#include "glm/vec2.hpp"

namespace ukariga {
    class Player;

    template<typename T>
    void swap_back_and_pop(std::vector<T>& vector, size_t index) {
        assert(index < vector.size());
        vector[index] = vector.back();
        vector.pop_back();
    }

    class BulletManager {
    public:
        explicit BulletManager(const float radius) : radius{radius}, radius_sq{radius * radius} {}

        void create_bullet(const glm::vec2 position, const glm::vec2 velocity) {
            positions.push_back(position);
            previous_positions.push_back(position);
            velocities.push_back(velocity);
        }

        void destroy_bullet(const size_t index) {
            swap_back_and_pop(positions, index);
            swap_back_and_pop(previous_positions, index);
            swap_back_and_pop(velocities, index);
        }

        void update(const float delta) {
            for (size_t i = 0; i < positions.size(); i++) {
                previous_positions[i] = positions[i];
            }

            for (size_t i = 0; i < positions.size(); i++) {
                positions[i] += velocities[i] * delta;
            }
        }

        void draw(float alpha) const;

        void collide_player(Player& player);
    private:

        float radius{};
        float radius_sq{};
        std::vector<glm::vec2> positions;
        std::vector<glm::vec2> previous_positions;
        std::vector<glm::vec2> velocities;
    };
}
