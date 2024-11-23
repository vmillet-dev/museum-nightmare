#pragma once
#include "Actor.hpp"
#include <SFML/Graphics.hpp>

namespace game {

class Player : public Actor {
public:
    Player(float x, float y, class Game& game);
    void render(sf::RenderWindow& window) override;
    void handleCollision(GameObject* other) override;
    sf::FloatRect getBounds() const override;

private:
    sf::RectangleShape shape;
    sf::Vector2u windowSize;  // Store window size
};

} // namespace game
