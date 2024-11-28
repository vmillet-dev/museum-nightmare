#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>
#include "objects/GameObject.hpp"
#include <vector>
#include <memory>

class Camera {
public:
    Camera(sf::RenderWindow& window);

    // Update target position for smooth following
    void follow(game::GameObject* target);

    // Update camera position and view
    void update(float deltaTime);

    // Update parallax layers based on camera position
    void updateParallaxLayers(const std::vector<std::unique_ptr<game::GameObject>>& layers);

    // Get current view for rendering
    const sf::View& getView() const;

private:
    sf::RenderWindow& window;
    sf::View view;
    game::GameObject* target;
    sf::Vector2f lastPosition;

    void updateViewPosition();
    static constexpr float SMOOTHING = 0.1f;
};

#endif // CAMERA_HPP
