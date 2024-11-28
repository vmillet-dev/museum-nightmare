#ifndef FLOOROBJECT_HPP
#define FLOOROBJECT_HPP

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

namespace game {

class FloorObject : public game::GameObject {
public:
    FloorObject(const sf::Texture& texture, const sf::IntRect& textureRect,
                const sf::Vector2f& position, b2WorldId worldId);
    ~FloorObject() override;

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;  // Made const
    void initPhysics(b2WorldId worldId) override;

private:
    sf::Sprite sprite;
    b2BodyId bodyId{b2_nullBodyId};
    void initializePhysics(b2WorldId worldId, const sf::Vector2f& position);
};

} // namespace game

#endif // FLOOROBJECT_HPP
