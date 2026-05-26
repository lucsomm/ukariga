#pragma once

#include "glm/vec2.hpp"
#include "raymath.h"

namespace ukariga {
inline Vector2 to_ray_vec(const glm::vec2& vec) {
    return Vector2{vec.x, vec.y};
}

inline glm::vec2 to_glm_vec(const Vector2& vec) {
    return glm::vec2{vec.x, vec.y};
}
}
