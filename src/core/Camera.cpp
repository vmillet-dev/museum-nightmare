#include "Camera.hpp"
#include <spdlog/spdlog.h>

namespace game {

Camera::Camera(sf::RenderWindow& window, sf::Vector2f initialPosition)
    : window(window), position(initialPosition) {
    view = window.getDefaultView();
    view.setCenter(initialPosition);
}

void Camera::update(const sf::Vector2f& targetPosition) {
    // Smoothly interpolate between current position and target position
    sf::Vector2f delta = targetPosition - position;
    position += delta / smoothFactor;

    view.setCenter(position);
    window.setView(view);
}

sf::View Camera::getView() const {
    return view;
}

sf::Vector2f Camera::getPosition() const {
    return position;
}

} // namespace game
