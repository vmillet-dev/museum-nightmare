#pragma once
#include "Actor.hpp"
#include <SFML/Graphics.hpp>
#include "../../../core/const.hpp"

namespace game {

class Player : public Actor {
public:
    Player(float x, float y, class Game& game);
    void render(sf::RenderWindow& window) override;
    void initPhysics(b2WorldId worldId) override;  // Updated to use Box2D v3.0.0 API

private:
    sf::RectangleShape shape;
    sf::Vector2u windowSize;  // Store window size
};

} // namespace game
