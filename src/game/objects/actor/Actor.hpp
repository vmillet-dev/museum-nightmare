#pragma once
#include "../GameObject.hpp"
#include "../../../input/providers/InputProvider.hpp"
#include "../../../core/const.hpp"
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
    virtual void render(sf::RenderWindow& window) override = 0;  // Removed const to match base class
    void initPhysics(b2WorldId worldId) override;  // Updated to use new Box2D v3.0.0 API
    void move(float x, float y, float deltaTime);
};

} // namespace game
