#pragma once
#include "raylib.h"
#include "state_machine.h"

namespace ukariga {
    class Game {
    public:
        struct Gameplay final : State {
            Gameplay();

            void update(float delta) override;

            void physics_update(float delta) override;

            void draw(float alpha) override;
        };

        static Game& get_singleton() {
            static Game game;
            return game;
        }

        explicit Game();

        Game(const Game&) = delete;

        Game& operator=(const Game&) = delete;

        Game& operator=(Game&&) = delete;

        Game(Game&&) = delete;

        ~Game();

        void run();

        [[nodiscard]] glm::ivec2 get_window_size() const {
            return window_size;
        }

        void set_window_size(int width, int height);

        void set_window_size(const glm::ivec2 size) {
            set_window_size(size.x, size.y);
        }

        // Start with empty state as to not init camera before window
        StateMachine<State, Gameplay> state_machine;
        Camera2D main_camera{};


    private:
        void update(float delta);

        void physics_update(float delta);

        void draw(float alpha);

        [[nodiscard]] double get_physics_dt() const {
            return 1. / physics_rate;
        }

        double physics_rate = 120.;
        glm::ivec2 window_size{720, 1080};
    };
}

