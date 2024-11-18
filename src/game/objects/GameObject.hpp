#pragma once
#include <SFML/Graphics.hpp>

namespace game {

class GameObject {
public:
    GameObject(float x, float y);
    virtual ~GameObject() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void handleCollision(GameObject* other) = 0;
    virtual sf::FloatRect getBounds() const = 0;

    void setPosition(const sf::Vector2f& newPosition);
    const sf::Vector2f& getPosition() const;

protected:
    sf::Vector2f position;
};

} // namespace game
