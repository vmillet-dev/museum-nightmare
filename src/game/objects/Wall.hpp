#pragma once
#include "GameObject.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Wall : public GameObject {
public:
    Wall(float x, float y, float width, float height);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void initPhysics(b2WorldId worldId) override;

private:
    sf::RectangleShape shape;
    float width;
    float height;

    static constexpr float WALL_FRICTION = 0.3f;
};

} // namespace game
