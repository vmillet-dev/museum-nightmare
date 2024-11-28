#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

namespace game {

class GameObject : public sf::Drawable {
public:
    GameObject(float x, float y);
    virtual ~GameObject();

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;  // Remove const to match derived implementations

    // Physics initialization with new Box2D v3.0.0 API
    virtual void initPhysics(b2WorldId worldId) = 0;

    const sf::Vector2f& getPosition() const;
    void setPosition(const sf::Vector2f& newPosition);
    b2BodyId getPhysicsBody() const { return bodyId; }

protected:
    sf::Vector2f position;
    b2BodyId bodyId;  // Box2D physics body ID
    b2WorldId worldId; // Store world ID for physics operations

    // Helper method to sync SFML position with Box2D body
    void syncPositionWithPhysics();

    // Implement sf::Drawable interface
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        const_cast<GameObject*>(this)->render(static_cast<sf::RenderWindow&>(target));
    }
};

} // namespace game
