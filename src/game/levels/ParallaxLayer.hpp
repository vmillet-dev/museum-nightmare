#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace game {

class ParallaxLayer {
public:
    ParallaxLayer(const std::string& texturePath, float parallaxFactor);
    ~ParallaxLayer() = default;

    void update(const sf::View& view);
    void render(sf::RenderWindow& window);

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float parallaxFactor;
    sf::Vector2f initialPosition;
};

} // namespace game
