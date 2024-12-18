#include "TGUIButtonWrapper.hpp"

namespace game {

const sf::Color TGUIButtonWrapper::defaultColor = sf::Color(100, 100, 100);
const sf::Color TGUIButtonWrapper::hoverColor = sf::Color(150, 150, 150);
const sf::Color TGUIButtonWrapper::selectedColor = sf::Color(200, 200, 200);

TGUIButtonWrapper::TGUIButtonWrapper(const std::string& buttonText, const sf::Vector2f& position, const sf::Vector2f& size) {
    button = tgui::Button::create(buttonText);
    button->setPosition({std::to_string(position.x), std::to_string(position.y)});
    button->setSize({std::to_string(size.x), std::to_string(size.y)});

    // Match original Button styling
    button->getRenderer()->setBackgroundColor(tgui::Color(defaultColor.r, defaultColor.g, defaultColor.b));
    button->getRenderer()->setBorderColor(tgui::Color::White);
    button->getRenderer()->setTextColor(tgui::Color::White);
    button->getRenderer()->setBorders(2);
    button->setTextSize(24);
    button->getRenderer()->setFont("assets/arial.ttf");
}

void TGUIButtonWrapper::update(InputManager& inputManager) {
    bool wasHovered = isHovered;
    isHovered = button->isMouseOnWidget();

    // Update color based on state
    tgui::Color bgColor;
    if (isSelected || isHovered) {
        bgColor = tgui::Color(selectedColor.r, selectedColor.g, selectedColor.b);
    } else {
        bgColor = tgui::Color(defaultColor.r, defaultColor.g, defaultColor.b);
    }
    button->getRenderer()->setBackgroundColor(bgColor);

    // Handle click using InputManager
    bool isPressed = inputManager.isActionPressed(Action::Fire) || inputManager.isActionPressed(Action::Confirm);
    clicked = (isSelected || isHovered) && isPressed && !wasPressed;
    wasPressed = isPressed;
}

void TGUIButtonWrapper::setSelected(bool selected) {
    isSelected = selected;
    button->setFocused(selected);
}

} // namespace game
