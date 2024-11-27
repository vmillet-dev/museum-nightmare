#include "Button.hpp"
#include <spdlog/spdlog.h>

namespace game {

Button::Button(const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size) {
    // Load font
    if (!font.loadFromFile("assets/arial.ttf")) {
        spdlog::error("Failed to load font in Button!");
    }

    // Setup shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setOrigin(size.x / 2, size.y / 2);
    shape.setFillColor(defaultColor);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);

    // Setup text
    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // Center text in button
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    text.setPosition(position.x, position.y);

    isHovered = false;
    clicked = false;
    wasPressed = false;
}

void Button::update(InputManager& inputManager) {
    // Get mouse position from window
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(inputManager.getWindow()));

    bool wasHovered = isHovered;
    isHovered = isMouseOver(mousePos);

    // Handle hover state change
    if (isHovered != wasHovered) {
        shape.setFillColor(isHovered ? hoverColor : defaultColor);
    }

    // Update color based on selection state
    if (isSelected) {
        shape.setFillColor(selectedColor);
    }

    // Handle click using InputManager (mouse or confirm action)
    bool isPressed = inputManager.isActionPressed(Action::MouseLeft) ||
                    (isSelected && inputManager.isActionPressed(Action::Confirm));
    clicked = (isHovered || isSelected) && isPressed && !wasPressed;
    wasPressed = isPressed;
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

} // namespace game
