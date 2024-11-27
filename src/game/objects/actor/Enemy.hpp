#pragma once
#include "Actor.hpp"
#include "../../../input/providers/RandomAIInputProvider.hpp"

namespace game {

class Enemy : public Actor {
public:
    Enemy(float x, float y);
    void render(sf::RenderWindow& window) override;
    void initPhysics(b2WorldId worldId) override;  // Updated to use Box2D v3.0.0 API

private:
    sf::RectangleShape shape;
    static constexpr float ENEMY_DENSITY = 1.0f;
    static constexpr float ENEMY_FRICTION = 0.3f;
};

} // namespace game
