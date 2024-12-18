#include "TGUIButtonWrapper.hpp"
#include <spdlog/spdlog.h>
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace game {

const sf::Color TGUIButtonWrapper::defaultColor = sf::Color(100, 100, 100);
const sf::Color TGUIButtonWrapper::selectedColor = sf::Color(150, 150, 150);

TGUIButtonWrapper::TGUIButtonWrapper(const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size) {
    button = tgui::Button::create(buttonText);

    // Match original Button positioning (centered)
    button->setPosition({position.x - size.x / 2.f, position.y - size.y / 2.f});
    button->setSize({size.x, size.y});

    // Match original Button text styling
    button->setTextSize(24);
    button->getRenderer()->setTextColor(tgui::Color::White);

    // Match original Button colors and outline
    button->getRenderer()->setBackgroundColor(tgui::Color(defaultColor.r, defaultColor.g, defaultColor.b));
    button->getRenderer()->setBorderColor(tgui::Color::White);
    button->getRenderer()->setBorders(2);

    // Disable TGUI's default hover effects to use our custom ones
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

    // Get mouse position relative to the window from InputManager
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(inputManager.getWindow()));
    isHovered = button->isMouseOnWidget(mousePos);

    // Update color based on state (match original Button behavior)
    tgui::Color bgColor = (isSelected || isHovered) ?
        tgui::Color(selectedColor.r, selectedColor.g, selectedColor.b) :
        tgui::Color(defaultColor.r, defaultColor.g, defaultColor.b);
    button->getRenderer()->setBackgroundColor(bgColor);

    // Handle click using InputManager (match original Button behavior)
    bool isPressed = inputManager.isActionPressed(Action::Fire) || inputManager.isActionPressed(Action::Confirm);
    clicked = (isSelected || isHovered) && isPressed && !wasPressed;
    wasPressed = isPressed;
}

void TGUIButtonWrapper::setSelected(bool selected) {
    isSelected = selected;
}

} // namespace game
