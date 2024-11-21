#pragma once
#include "GameObject.hpp"
#include "../../input/InputManager.hpp"
#include <SFML/Graphics.hpp>

namespace game {

class Player : public GameObject {
public:
    Player(float x, float y);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleCollision(GameObject* other) override;
    sf::FloatRect getBounds() const override;
    void move(float dx, float dy, float deltaTime);
    void setVelocity(const sf::Vector2f& newVelocity);

    // New input handling methods
    void bindInputActions();
    void handleMoveAction(Action action);
    void handleInteractAction(Action action);

private:
    sf::RectangleShape shape;
    float speed;
    sf::Vector2f velocity;
    sf::Vector2u windowSize;  // Store window size
    bool isMoving;
};

} // namespace game
