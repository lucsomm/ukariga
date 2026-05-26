#pragma once
#include "entity.h"
#include "physics.h"
#include "state_machine.h"

namespace ukariga {
class Player : public Entity<Player> {
public:
    void update_impl(float delta);
    void physics_update_impl(float delta);
    void draw_impl(glm::vec2 draw_position);

private:
    class Flying final : public State {
    public:
        static constexpr float FLY_SPEED = 200.f;

        explicit Flying(Player& player) : player(player) {}

        void update(float delta) override;
        void physics_update(float delta) override;
        void draw(glm::vec2 draw_position) override;

    private:
        glm::vec2 input_dir{};
        Player& player;
    };

    glm::vec2 velocity{};
    StateMachine<Flying> state{*this};
};
}
