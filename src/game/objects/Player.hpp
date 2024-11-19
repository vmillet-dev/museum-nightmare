#pragma once
#include "GameObject.hpp"
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

private:
    sf::RectangleShape shape;
    float speed;
    sf::Vector2f velocity;
    sf::Vector2u windowSize;  // Store window size
};

} // namespace game
