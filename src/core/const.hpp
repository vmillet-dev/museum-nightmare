#pragma once

namespace Constants {
    namespace Physics {
        // World physics constants
        static constexpr float GRAVITY_Y = 9.8f;
        static constexpr int32_t VELOCITY_ITERATIONS = 8;
        static constexpr int32_t POSITION_ITERATIONS = 3;

        // Object physics properties
        static constexpr float ACTOR_DENSITY = 1.0f;
        static constexpr float PLAYER_DENSITY = ACTOR_DENSITY;
        static constexpr float ENEMY_DENSITY = ACTOR_DENSITY;

        static constexpr float ACTOR_FRICTION = 0.3f;
        static constexpr float PLAYER_FRICTION = ACTOR_FRICTION;
        static constexpr float ENEMY_FRICTION = ACTOR_FRICTION;
        static constexpr float WALL_FRICTION = 0.3f;

        // Object dimensions
        static constexpr float ACTOR_SIZE = 16.0f;  // Half-size of actor's box shape
        static constexpr float WALL_THICKNESS = 32.0f;  // Wall thickness
        static constexpr float WALL_HALF_THICKNESS = WALL_THICKNESS / 2.0f;  // Half thickness for positioning
    }
}
