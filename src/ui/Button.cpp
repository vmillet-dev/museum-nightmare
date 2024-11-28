#include "Button.hpp"
#include <spdlog/spdlog.h>

namespace game {

Button::Button(const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size, const core::ResourceManager& resourceManager) {
    // Get font from ResourceManager
    const sf::Font* buttonFont = resourceManager.getFont("arial");
    if (!buttonFont) {
        spdlog::error("Failed to get arial font from ResourceManager in Button!");
        return;
    }

    // Setup shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setOrigin(size.x / 2, size.y / 2);
    shape.setFillColor(defaultColor);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);

    // Setup text
    text.setFont(*buttonFont);
    text.setString(buttonText);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // Center text in button
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    text.setPosition(position.x, position.y);

    isHovered = false;
}

void Button::handleInput(const sf::Vector2f& mousePos) {
    bool wasHovered = isHovered;
    isHovered = isMouseOver(mousePos);

    // Handle hover state change
    if (isHovered != wasHovered) {
        shape.setFillColor(isHovered ? hoverColor : defaultColor);
    }

    // Handle click
    if (isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        handleClick(mousePos);
    }
}

void Button::setCallback(std::function<void()> newCallback) {
    callback = std::move(newCallback);
}

void Button::handleClick(const sf::Vector2f& mousePos) {
    if (callback) {
        callback();
    }
}

void Button::handleHover(const sf::Vector2f& mousePos) {
    isHovered = isMouseOver(mousePos);
    shape.setFillColor(isHovered ? hoverColor : defaultColor);
}

void Button::update(float deltaTime) {
    // Animation or other updates can be added here
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

} // namespace game
