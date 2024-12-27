#include "Button.hpp"
#include <spdlog/spdlog.h>

namespace game {

Button::Button(const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size) {
    // Load font first
    if (!font.openFromFile("assets/arial.ttf")) {
        spdlog::error("Failed to load font in Button!");
    }

    // Setup shape
    shape.setSize(size);
    shape.setPosition(position);
    shape.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
    shape.setFillColor(defaultColor);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);

    // Setup text with font after loading
    text = std::make_unique<sf::Text>(font, buttonText, 24);
    text->setFillColor(sf::Color::White);

    // Center text in button
    sf::FloatRect textBounds = text->getLocalBounds();
    text->setOrigin(sf::Vector2f(textBounds.size.x / 2.f, textBounds.size.y / 2.f));
    text->setPosition({ position.x, position.y });

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
    shape.setFillColor(isHovered || isSelected ? selectedColor : defaultColor);

    // Update color based on selection state
    //shape.setFillColor(isSelected || isHovered ? selectedColor : defaultColor);

    // Handle click using InputManager (mouse or confirm action)
    bool isPressed = inputManager.isActionPressed(Action::Fire) || inputManager.isActionPressed(Action::Confirm);
    clicked = (isSelected || isHovered) && isPressed && !wasPressed;
    wasPressed = isPressed;
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(*text);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

} // namespace game
