#include "Button.hpp"
#include <spdlog/spdlog.h>
#ifdef _WIN32
#include <corecrt_math_defines.h>
#endif
#include <cmath>

namespace game {

Button::Button(const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size) {
    // Load font
    if (!font.loadFromFile("assets/arial.ttf")) {
        spdlog::error("Failed to load font in Button!");
    }

    // Setup shape with enhanced visuals
    shape.setSize(size);
    shape.setPosition(position);
    shape.setOrigin(size.x / 2, size.y / 2);
    shape.setFillColor(defaultColor);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(outlineDefaultColor);

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
    effectClock.restart();
}

void Button::update(InputManager& inputManager) {
    float deltaTime = effectClock.restart().asSeconds();

    // Get mouse position from window
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(inputManager.getWindow()));

    bool wasHovered = isHovered;
    isHovered = isMouseOver(mousePos);

    // Handle input state
    bool isPressed = inputManager.isActionPressed(Action::Fire) ||
                    inputManager.isActionPressed(Action::Confirm);
    bool justPressed = inputManager.isActionJustPressed(Action::Fire) ||
                      inputManager.isActionJustPressed(Action::Confirm);

    if (justPressed) {
        spdlog::debug("Button '{}' just pressed - Selected: {}, Hovered: {}, WasPressed: {}",
            text.getString().toAnsiString(), isSelected, isHovered, wasPressed);
    }

    clicked = (isSelected || isHovered) && justPressed;

    if (clicked) {
        spdlog::info("Button '{}' clicked - Triggering callback", text.getString().toAnsiString());
        if (onClick) {
            onClick();
        } else {
            spdlog::error("Button '{}' clicked but no callback registered!", text.getString().toAnsiString());
        }
    }

    wasPressed = isPressed;

    // Update visual effects
    updateEffects(deltaTime);
    updateVisualState();
}

void Button::updateEffects(float deltaTime) {
    // Pulse effect for selected state
    if (isSelected) {
        pulseEffect += deltaTime * 4.0f;
        if (pulseEffect > 2 * M_PI) pulseEffect -= 2 * M_PI;
    } else {
        pulseEffect = 0.0f;
    }

    // Smooth transition effect
    float targetTransition = (isHovered || isSelected) ? 1.0f : 0.0f;
    float transitionSpeed = 5.0f;

    if (transitionEffect < targetTransition) {
        transitionEffect = std::min(transitionEffect + deltaTime * transitionSpeed, targetTransition);
    } else if (transitionEffect > targetTransition) {
        transitionEffect = std::max(transitionEffect - deltaTime * transitionSpeed, targetTransition);
    }
}

void Button::updateVisualState() {
    sf::Color currentColor = defaultColor;
    sf::Color currentOutline = outlineDefaultColor;

    // Determine base color based on state
    if (wasPressed && (isHovered || isSelected)) {
        currentColor = pressedColor;
    } else if (isSelected) {
        currentColor = selectedColor;
        currentOutline = outlineSelectedColor;
    } else if (isHovered) {
        currentColor = hoverColor;
    }

    // Apply pulse effect when selected
    if (isSelected) {
        float pulse = (std::sin(pulseEffect) + 1.0f) * 0.5f;
        currentOutline.a = static_cast<sf::Uint8>(155 + 100 * pulse);
        shape.setOutlineThickness(2.0f + pulse * 2.0f);
    } else {
        shape.setOutlineThickness(2.0f);
    }

    // Apply transition effect
    sf::Color finalColor = sf::Color(
        currentColor.r,
        currentColor.g,
        currentColor.b,
        currentColor.a
    );

    shape.setFillColor(finalColor);
    shape.setOutlineColor(currentOutline);
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

} // namespace game
