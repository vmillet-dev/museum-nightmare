#pragma once
#include <box2d/box2d.h>
#include "ContactListener.hpp"
#include <memory>
#include <cstdint>
#include "../../core/const.hpp"

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
};

} // namespace game
