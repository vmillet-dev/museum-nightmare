#pragma once
#include "GameObject.hpp"
#include "../../core/const.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Wall : public GameObject {
public:
    Wall(float x, float y, float width, float height);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;  // Removed const to match base class
    void initPhysics(b2WorldId worldId) override;

private:
    sf::RectangleShape shape;
    float width;
    float height;
};

} // namespace game
