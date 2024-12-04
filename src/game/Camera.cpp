#include "Camera.hpp"
#include <algorithm>

namespace game {

Camera::Camera(float width, float height)
    : view(sf::Vector2f(width / 2.0f, height / 2.0f), sf::Vector2f(width, height))
    , targetPosition(width / 2.0f, height / 2.0f) {}

void Camera::move(const sf::Vector2f& delta) {
    setTargetPosition(targetPosition + delta);
}

void Camera::setPosition(const sf::Vector2f& position) {
    setTargetPosition(position);
    view.setCenter(position);
    enforceBounds();
}

void Camera::setCenter(const sf::Vector2f& center) {
    view.setCenter(center);
    enforceBounds();
}

const sf::View& Camera::getView() const {
    return view;
}

void Camera::handleResize(const sf::RenderWindow& window) {
    float windowRatio = window.getSize().x / static_cast<float>(window.getSize().y);
    float viewRatio = view.getSize().x / view.getSize().y;
    float sizeX = 1.0f;
    float sizeY = 1.0f;
    float posX = 0.0f;
    float posY = 0.0f;

    if (windowRatio > viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1.0f - sizeX) / 2.0f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1.0f - sizeY) / 2.0f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
}

void Camera::setZoom(float factor) {
    currentZoom = std::max(0.1f, factor);
    view.setSize(view.getSize().x * currentZoom, view.getSize().y * currentZoom);
    enforceBounds();
}

void Camera::zoom(float factor) {
    setZoom(currentZoom * factor);
}

void Camera::setBounds(const sf::FloatRect& newBounds) {
    bounds = newBounds;
    hasBounds = true;
    enforceBounds();
}

void Camera::setTargetPosition(const sf::Vector2f& target) {
    targetPosition = target;
}

void Camera::update(float deltaTime) {
    if (view.getCenter() != targetPosition) {
        sf::Vector2f newPos = view.getCenter();
        sf::Vector2f diff = targetPosition - newPos;
        newPos += diff * std::min(1.0f, deltaTime * smoothSpeed);
        view.setCenter(newPos);
        enforceBounds();
    }
}

void Camera::setParallaxFactor(float factor) {
    parallaxFactor = factor;
}

float Camera::getParallaxFactor() const {
    return parallaxFactor;
}

void Camera::enforceBounds() {
    if (!hasBounds) return;

    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();
    sf::Vector2f halfSize = size / 2.0f;

    // Enforce horizontal bounds
    if (center.x - halfSize.x < bounds.left) {
        center.x = bounds.left + halfSize.x;
    } else if (center.x + halfSize.x > bounds.left + bounds.width) {
        center.x = bounds.left + bounds.width - halfSize.x;
    }

    // Enforce vertical bounds
    if (center.y - halfSize.y < bounds.top) {
        center.y = bounds.top + halfSize.y;
    } else if (center.y + halfSize.y > bounds.top + bounds.height) {
        center.y = bounds.top + bounds.height - halfSize.y;
    }

    view.setCenter(center);
}

} // namespace game
