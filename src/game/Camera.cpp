#include "Camera.hpp"
#include "objects/ParallaxBackground.hpp"

Camera::Camera(sf::RenderWindow& window)
    : window(window)
    , target(nullptr)
    , lastPosition(0.f, 0.f) {
    view = window.getDefaultView();
}

void Camera::follow(game::GameObject* target) {
    this->target = target;
    if (target) {
        lastPosition = target->getPosition();
        view.setCenter(lastPosition);
    }
}

void Camera::update(float deltaTime) {
    if (target) {
        updateViewPosition();
    }
}

void Camera::updateParallaxLayers(const std::vector<std::unique_ptr<game::GameObject>>& layers) {
    const sf::Vector2f viewCenter = view.getCenter();

    for (const auto& layer : layers) {
        if (auto* parallaxLayer = dynamic_cast<game::ParallaxBackground*>(layer.get())) {
            // Update each layer's position based on its parallax factor
            parallaxLayer->setViewCenter(viewCenter);
        }
    }
}

void Camera::updateViewPosition() {
    if (!target) return;

    const sf::Vector2f targetPos = target->getPosition();

    // Smooth camera movement using constant smoothing factor
    sf::Vector2f newPosition = lastPosition + (targetPos - lastPosition) * SMOOTHING;

    // Update view position
    view.setCenter(newPosition);
    window.setView(view);
    lastPosition = newPosition;
}

const sf::View& Camera::getView() const {
    return view;
}
