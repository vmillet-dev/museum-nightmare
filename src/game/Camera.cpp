#include "Camera.hpp"

namespace game {

Camera::Camera(float width, float height)
    : view(sf::Vector2f(width / 2.0f, height / 2.0f), sf::Vector2f(width, height)) {}

void Camera::move(const sf::Vector2f& delta) {
    view.move(delta * parallaxFactor);
}

void Camera::setPosition(const sf::Vector2f& position) {
    view.setCenter(position * parallaxFactor);
}

const sf::View& Camera::getView() const {
    return view;
}

void Camera::setParallaxFactor(float factor) {
    parallaxFactor = factor;
}

float Camera::getParallaxFactor() const {
    return parallaxFactor;
}

} // namespace game
