#pragma once
#include "Actor.hpp"
#include "../../../input/providers/RandomAIInputProvider.hpp"
#include "../../../core/const.hpp"

namespace game {

class Enemy : public Actor {
public:
    Enemy(float x, float y);
    void render(sf::RenderWindow& window) const override;  // Made const
    void initPhysics(b2WorldId worldId) override;  // Updated to use Box2D v3.0.0 API

private:
    sf::RectangleShape shape;
};

} // namespace game
