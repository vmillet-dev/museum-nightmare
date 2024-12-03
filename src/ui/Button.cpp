#include "Button.hpp"

namespace game {

Button::Button(const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size, const WidgetStyle& style)
    : style_(style) {
    // Load font
    if (!font_.loadFromFile(style_.fontPath)) {
        spdlog::error("Failed to load font in Button!");
    }

    // Setup shape
    shape_.setSize(size);
    shape_.setPosition(position);
    shape_.setOrigin(size.x / 2, size.y / 2);
    shape_.setFillColor(style_.defaultColor);
    shape_.setOutlineThickness(style_.outlineThickness);
    shape_.setOutlineColor(style_.outlineColor);

    // Store widget properties
    position_ = position;
    size_ = size;

    // Setup text
    text_.setFont(font_);
    setText(buttonText);
}

void Button::update(InputManager& inputManager) {
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(inputManager.getWindow()));

    isHovered_ = isMouseOver(mousePos);

    // Update visual state
    shape_.setFillColor(isHovered_ || isSelected_ ? style_.selectedColor : style_.defaultColor);

    // Handle click using InputManager (mouse or confirm action)
    bool isPressed = inputManager.isActionPressed(Action::Fire) || inputManager.isActionPressed(Action::Confirm);
    clicked_ = (isSelected_ || isHovered_) && isPressed && !wasPressed_;
    wasPressed_ = isPressed;
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape_);
    window.draw(text_);
}

void Button::setText(const std::string& text) {
    text_.setString(text);
    text_.setCharacterSize(style_.characterSize);
    text_.setFillColor(style_.textColor);
    updateTextPosition();
}

void Button::updateTextPosition() {
    sf::FloatRect textBounds = text_.getLocalBounds();
    text_.setOrigin(textBounds.width / 2, textBounds.height / 2);
    text_.setPosition(position_);
}

} // namespace game
