#include "game.h"

#include <algorithm>
#include <chrono>

#include "raylib.h"
#include "entity.h"
#include "player.h"

ukariga::Game::Gameplay::Gameplay() {
    const auto window = glm::vec2{get_singleton().get_window_size()};

    get_singleton().main_camera.target = Vector2{0, 0};
    get_singleton().main_camera.offset = (Vector2){window.x / 2.0f, window.y / 2.0f};
    get_singleton().main_camera.rotation = 0.0f;
    get_singleton().main_camera.zoom = 1.0f;

    auto& player = Player::create();

    // PlaySound(LoadSound(RES_PATH"test.wav"));
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
    InitWindow(window_size.x, window_size.y, "Ukariga");

    // InitAudioDevice();

    const auto monitor = GetCurrentMonitor();

    if (monitor < 0) {
        SetTargetFPS(static_cast<int>(physics_rate));
        return;
    }

    if (const auto monitor_rate = GetMonitorRefreshRate(monitor); monitor_rate > static_cast<int>(physics_rate)) {
        SetTargetFPS(monitor_rate);
    }
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

void ukariga::Game::set_window_size(int width, int height) {
    window_size = {width, height};
    SetWindowSize(window_size.x, window_size.y);
}

