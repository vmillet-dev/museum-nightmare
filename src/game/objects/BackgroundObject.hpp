#pragma once
#include "GameObject.hpp"
#include "../Camera.hpp"
#include <SFML/Graphics.hpp>

namespace game {

class BackgroundObject : public GameObject {
public:
    BackgroundObject(float x, float y, float depth, const std::string& texturePath, Camera& camera);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void initPhysics(b2WorldId worldId) override;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float depth;
    Camera& camera;
    sf::Vector2f initialPosition;
};

} // namespace game
