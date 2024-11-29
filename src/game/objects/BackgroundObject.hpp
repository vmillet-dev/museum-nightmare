#pragma once
#include "GameObject.hpp"
#include <SFML/Graphics.hpp>
#include "../Camera.hpp"

namespace game {

class BackgroundObject : public GameObject {
public:
    BackgroundObject(float x, float y, float depth, const std::string& texturePath, const Camera& camera);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void initPhysics(b2WorldId worldId) override;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float depth;
    sf::Vector2f initialPosition;
    const Camera& camera;  // Store reference to camera
};

} // namespace game
