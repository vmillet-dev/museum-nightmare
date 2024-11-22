#pragma once
#include "Actor.hpp"
#include "../../input/providers/RandomAIInputProvider.hpp"

namespace game {

class Enemy : public Actor {
public:
    Enemy(float x, float y);
    void render(sf::RenderWindow& window) override;
    void handleCollision(GameObject* other) override;
    sf::FloatRect getBounds() const override;

private:
    sf::RectangleShape shape;
};

} // namespace game
