#include "Button.hpp"
#include <spdlog/spdlog.h>

namespace game {

Button::Button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size)
    : position(position), size(size) {

    // Load font
    if (!font.loadFromFile("resources/arial.ttf")) {
        spdlog::error("Failed to load font in Button!");
    }

    // Setup shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setOrigin(size.x / 2, size.y / 2);
    shape.setFillColor(sf::Color(100, 100, 100));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);

    // Setup text
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);

    // Center text in button
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    buttonText.setPosition(position.x, position.y);
}

void Button::setCallback(const std::function<void()>& callback) {
    onClick = callback;
}

void Button::handleClick(const sf::Vector2f& mousePos) {
    if (isMouseOver(mousePos) && onClick) {
        onClick();
    }
}

void Button::handleHover(const sf::Vector2f& mousePos) {
    if (isMouseOver(mousePos)) {
        shape.setFillColor(sf::Color(150, 150, 150));
    } else {
        shape.setFillColor(sf::Color(100, 100, 100));
    }
}

void Button::update(float deltaTime) {
    // Animation or other updates can be added here
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(buttonText);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    sf::FloatRect bounds = shape.getGlobalBounds();
    return bounds.contains(mousePos);
}

} // namespace game
