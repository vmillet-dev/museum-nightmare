#include "TGUIButtonWrapper.hpp"
#include <spdlog/spdlog.h>
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace game {

const sf::Color TGUIButtonWrapper::defaultColor = sf::Color(100, 100, 100);
const sf::Color TGUIButtonWrapper::selectedColor = sf::Color(150, 150, 150);

TGUIButtonWrapper::TGUIButtonWrapper(const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size) {
    button = tgui::Button::create(buttonText);

    // Match original Button positioning and size
    button->setSize({size.x, size.y});
    button->setPosition({position.x - size.x / 2.f, position.y - size.y / 2.f});

    // Match original Button text styling
    button->setTextSize(24);
    button->getRenderer()->setTextColor(tgui::Color::White);

    // Center text using the same logic as original Button
    auto textSize = button->getFullSize();
    button->setTextSize(24); // Reset size after getting full size
    float xOffset = (size.x - textSize.x) / 2.f;
    float yOffset = (size.y - textSize.y) / 2.f;
    button->getRenderer()->setPadding({xOffset, yOffset});

    // Match original Button colors and outline
    button->getRenderer()->setBackgroundColor(tgui::Color(defaultColor.r, defaultColor.g, defaultColor.b));
    button->getRenderer()->setBorderColor(tgui::Color::White);
    button->getRenderer()->setBorders(2);

    // Disable TGUI's default hover effects to match original Button behavior
    button->getRenderer()->setBackgroundColorHover(tgui::Color(defaultColor.r, defaultColor.g, defaultColor.b));
    button->getRenderer()->setBackgroundColorDown(tgui::Color(defaultColor.r, defaultColor.g, defaultColor.b));
    button->getRenderer()->setBorderColorHover(tgui::Color::White);
    button->getRenderer()->setBorderColorDown(tgui::Color::White);

    isHovered = false;
    clicked = false;
    wasPressed = false;
}

void TGUIButtonWrapper::update(InputManager& inputManager) {
    bool wasHovered = isHovered;

    // Get mouse position relative to the window from InputManager (match original Button)
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(inputManager.getWindow()));
    isHovered = button->isMouseOnWidget(mousePos);

    // Update color based on state (exact match with original Button behavior)
    if (isHovered || isSelected) {
        button->getRenderer()->setBackgroundColor(tgui::Color(selectedColor.r, selectedColor.g, selectedColor.b));
    } else {
        button->getRenderer()->setBackgroundColor(tgui::Color(defaultColor.r, defaultColor.g, defaultColor.b));
    }

    // Handle click using InputManager (match original Button behavior)
    bool isPressed = inputManager.isActionPressed(Action::Fire) || inputManager.isActionPressed(Action::Confirm);
    clicked = (isSelected || isHovered) && isPressed && !wasPressed;
    wasPressed = isPressed;
}

void TGUIButtonWrapper::setSelected(bool selected) {
    isSelected = selected;
}

} // namespace game
