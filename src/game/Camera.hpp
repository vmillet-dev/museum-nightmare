#pragma once
#include <SFML/Graphics.hpp>

namespace game {

class Camera {
public:
    Camera(float width, float height);

    void move(const sf::Vector2f& delta);
    void setPosition(const sf::Vector2f& position);
    void setCenter(const sf::Vector2f& center);
    const sf::View& getView() const;

    // Window management
    void handleResize(const sf::RenderWindow& window);

    // Camera controls
    void setZoom(float factor);
    void zoom(float factor);
    void setBounds(const sf::FloatRect& bounds);

    // Smooth movement
    void setTargetPosition(const sf::Vector2f& target);
    void update(float deltaTime);

    // Parallax support
    void setParallaxFactor(float factor);
    float getParallaxFactor() const;

private:
    sf::View view;
    sf::FloatRect bounds;
    sf::Vector2f targetPosition;
    float currentZoom{1.0f};
    float parallaxFactor{1.0f};
    float smoothSpeed{5.0f};
    bool hasBounds{false};

    void enforceBounds();
};

} // namespace game
