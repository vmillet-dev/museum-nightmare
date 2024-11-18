#pragma once
#include "GameObject.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Wall : public GameObject {
public:
    Wall(float x, float y, float width, float height);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleCollision(GameObject* other) override;
    sf::FloatRect getBounds() const override;

private:
    sf::RectangleShape shape;
};

} // namespace game
