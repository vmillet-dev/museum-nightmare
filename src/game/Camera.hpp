#pragma once
#include <SFML/Graphics.hpp>

namespace game {

class Camera {
public:
    Camera(float width, float height);

    void move(const sf::Vector2f& delta);
    void setPosition(const sf::Vector2f& position);
    const sf::View& getView() const;

    // Parallax support
    void setParallaxFactor(float factor);
    float getParallaxFactor() const;

private:
    sf::View view;
    float parallaxFactor{1.0f}; // Default to no parallax effect
};

} // namespace game
