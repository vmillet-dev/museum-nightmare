#pragma once
#include <box2d/box2d.h>
#include "ContactListener.hpp"
#include <memory>
#include <cstdint>

namespace game {

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void update(float deltaTime);
    b2WorldId getWorld() const { return worldId; }

    // Helper methods for common physics operations
    b2BodyId createBody(const b2BodyDef* bodyDef);
    void destroyBody(b2BodyId bodyId);

private:
    b2WorldId worldId;
    static constexpr float GRAVITY_Y = 9.8f;
    static constexpr int32_t VELOCITY_ITERATIONS = 8;
    static constexpr int32_t POSITION_ITERATIONS = 3;
};

} // namespace game
