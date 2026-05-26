#include "game.h"

#include <algorithm>
#include <chrono>

#include "raylib.h"
#include "entity.h"
#include "player.h"

ukariga::Game::Gameplay::Gameplay() {
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    get_singleton().main_camera.target = Vector2{0, 0};
    get_singleton().main_camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    get_singleton().main_camera.rotation = 0.0f;
    get_singleton().main_camera.zoom = 1.0f;

    auto& player = Player::create();
}

void ukariga::Game::Gameplay::update(const float delta) {
    EntityBase::update_all(delta);
}

void ukariga::Game::Gameplay::physics_update(const float delta) {
    EntityBase::physics_update_all(delta);
}

void ukariga::Game::Gameplay::draw(const float alpha) {
    ClearBackground(Color{50, 50, 50, 255});

    BeginMode2D(get_singleton().main_camera);

    EntityBase::draw_all(alpha);

    EndMode2D();
}

ukariga::Game::Game() {
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Ukariga");

    const auto monitor = GetCurrentMonitor();

    if (monitor < 0) {
        SetTargetFPS(60);
        return;
    }

    SetTargetFPS(GetMonitorRefreshRate(monitor));
}

ukariga::Game::~Game() {
    CloseWindow();
}

void ukariga::Game::update(const float delta) {
    state_machine.get_current_state().update(delta);
}

void ukariga::Game::physics_update(const float delta) {
    state_machine.get_current_state().physics_update(delta);
}

void ukariga::Game::draw(const float alpha) {
    BeginDrawing();

    state_machine.get_current_state().draw(alpha);

    EndDrawing();
}

void ukariga::Game::run() {
    state_machine.change_state<Gameplay>();

    double physics_accumulator = 0.;

    using clock = std::chrono::high_resolution_clock;
    auto previous_time = clock::now();

    while (!WindowShouldClose()) {
        constexpr double SPIRAL_OF_DEATH_THRESHOLD = .2;

        const auto current_time = clock::now();
        const std::chrono::duration<double> time_span = current_time - previous_time;
        const double dt = time_span.count();
        previous_time = current_time;

        const double physics_dt = get_physics_dt();
        physics_accumulator = std::min(physics_accumulator + dt, SPIRAL_OF_DEATH_THRESHOLD);

        update(static_cast<float>(dt));

        while (physics_accumulator >= physics_dt) {
            physics_update(static_cast<float>(physics_dt));
            physics_accumulator -= physics_dt;
        }

        const auto alpha = physics_accumulator / physics_dt;

        draw(static_cast<float>(alpha));
    }
}

