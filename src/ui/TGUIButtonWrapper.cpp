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

    // Set button origin and position (matching original Button implementation)
    button->setOrigin({size.x / 2.f, size.y / 2.f});
    button->setPosition({position.x, position.y});

    // Set text properties to match original Button implementation
    button->setTextSize(24); // Match original Button text size
    button->getRenderer()->setTextColor(tgui::Color::White);
    button->getRenderer()->setTextStyle(tgui::TextStyle::Regular);

    // Center text using relative positioning and origin (matching original Button implementation)
    button->setTextPosition({"50%", "50%"}, {0.5f, 0.5f});

    // Match original Button colors and outline
    button->getRenderer()->setBackgroundColor(tgui::Color(defaultColor.r, defaultColor.g, defaultColor.b));
    button->getRenderer()->setBorderColor(tgui::Color::White);
    button->getRenderer()->setBorderColorHover(tgui::Color::White);
    button->getRenderer()->setBorderColorFocused(tgui::Color::White);
    button->getRenderer()->setBorders({2, 2, 2, 2}); // Match original 2px outline thickness

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
