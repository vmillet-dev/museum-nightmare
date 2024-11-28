#pragma once
#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace game {

class Camera {
public:
    Camera(sf::RenderWindow& window, sf::Vector2f initialPosition);
    void update(const sf::Vector2f& targetPosition);
    sf::View getView() const;
    sf::Vector2f getPosition() const;

private:
    sf::RenderWindow& window;
    sf::View view;
    float smoothFactor = 5.0f; // Adjust for camera smoothing
    sf::Vector2f position;
};

} // namespace game
