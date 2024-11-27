#pragma once
#include "../GameObject.hpp"
#include "../../../input/providers/InputProvider.hpp"
#include <memory>

namespace game {

class Actor : public GameObject {
protected:
    std::unique_ptr<InputProvider> inputProvider;
    float speed;

public:
    Actor(float x, float y, std::unique_ptr<InputProvider> provider, float speed = 200.0f);
    virtual ~Actor() = default;

    void update(float deltaTime) override;
    void initPhysics(b2WorldId worldId) override;  // Updated to use new Box2D v3.0.0 API
    void move(float x, float y, float deltaTime);

protected:
    // Box2D specific properties
    static constexpr float DENSITY = 1.0f;
    static constexpr float FRICTION = 0.3f;
    static constexpr float ACTOR_SIZE = 16.0f; // Half-width/height in pixels
};

} // namespace game
