#pragma once
#include "GameObject.hpp"
#include "../../core/const.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Wall : public GameObject {
public:
    Wall(b2BodyId bodyId, float width, float height);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void initPhysics(b2WorldId worldId) override;

private:
    sf::RectangleShape shape;
    float width;
    float height;
};

} // namespace game
