#pragma once
#include "../states/ActorState.hpp"
#include "../../input/IInputProvider.hpp"
#include "GameObject.hpp"
#include <memory>

namespace game {

struct ActorProperties {
    float maxSpeed = 200.0f;
    float acceleration = 1000.0f;
    float deceleration = 2000.0f;
    float jumpForce = 400.0f;
    float gravity = 800.0f;
};

class Actor : public GameObject {
public:
    Actor(float x, float y);
    virtual ~Actor() = default;

    // GameObject interface implementation
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleCollision(GameObject* other) override;
    sf::FloatRect getBounds() const override;

    // Actor-specific methods
    void setInputProvider(std::unique_ptr<IInputProvider> provider);
    void setState(std::unique_ptr<ActorState> newState);
    ActorStateType getCurrentStateType() const;
    void setProperties(const ActorProperties& props);
    const ActorProperties& getProperties() const;

protected:
    std::unique_ptr<IInputProvider> inputProvider;
    std::unique_ptr<ActorState> currentState;
    ActorProperties properties;
    sf::Vector2f velocity;
    sf::RectangleShape shape;  // Basic shape for rendering
    float deltaTime;  // Store deltaTime for use in processInput

    virtual void handleStateTransition(ActorStateType newState);
    virtual void updatePhysics(float deltaTime);
    virtual void processInput();
    float moveTowards(float current, float target, float maxDelta);

    friend class ActorState;  // Allow states to access protected members
};

} // namespace game
