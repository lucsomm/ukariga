#include "player.h"

#include "raylib.h"
#include "math_utils.h"

void ukariga::Player::update_impl(const float delta) {
    state.get_current_state().update(delta);
}

void ukariga::Player::physics_update_impl(const float delta) {
    state.get_current_state().physics_update(delta);
    position += velocity * delta;
}

void ukariga::Player::draw_impl(const glm::vec2 draw_position) {
    state.get_current_state().draw(draw_position);
}

void ukariga::Player::on_hit() {
}

void ukariga::Player::Flying::update(const float delta) {
    input_dir = glm::vec2{
        static_cast<float>(IsKeyDown(KEY_D)) - static_cast<float>(IsKeyDown(KEY_A)),
        static_cast<float>(IsKeyDown(KEY_S)) - static_cast<float>(IsKeyDown(KEY_W))
    };
}

void ukariga::Player::Flying::physics_update(const float delta) {
    player.velocity.x = input_dir.x * FLY_SPEED;
}

void ukariga::Player::Flying::draw(const glm::vec2 draw_position) {
    DrawCircleV(to_ray_vec(draw_position), 32.f, SKYBLUE);
}
