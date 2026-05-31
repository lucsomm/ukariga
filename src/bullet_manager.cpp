#include "bullet_manager.h"

#include "raylib.h"
#include "math_utils.h"
#include "player.h"

void ukariga::BulletManager::draw(const float alpha) const {
    for (size_t i = 0; i < positions.size(); i++) {
        const auto draw_pos = previous_positions[i] + (positions[i] - previous_positions[i]) * alpha;
        DrawRectangleV(to_ray_vec(draw_pos), Vector2{radius, radius}, RED);
    }
}

void ukariga::BulletManager::collide_player(Player& player) {
    for (size_t i = 0; i < positions.size(); i++) {
        const auto dist = player.position - positions[i];
        const auto d = dist.x * dist.x + dist.y + dist.y;
        if (d <= radius_sq + Player::RADIUS) {
            player.on_hit();
            destroy_bullet(i);
        }
    }
}
