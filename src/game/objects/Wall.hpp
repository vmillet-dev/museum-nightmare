#pragma once
#include "GameObject.hpp"
#include "../../core/const.hpp"
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>

namespace game {

class Wall : public GameObject {
public:
    Wall(float x, float y, float width, float height, const std::string& texturePath);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void initPhysics(b2WorldId worldId) override;

private:
    sf::Sprite sprite;
    float width;
    float height;
};

} // namespace game
