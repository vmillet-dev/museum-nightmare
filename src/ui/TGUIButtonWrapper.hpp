#pragma once
#include <TGUI/Widgets/Button.hpp>
#include "../input/InputManager.hpp"
#include <SFML/Graphics.hpp>

namespace game {

class TGUIButtonWrapper {
public:
    TGUIButtonWrapper(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size);
    void update(InputManager& inputManager);
    bool wasClicked() const { return clicked; }
    void setSelected(bool selected);
    bool getSelected() const { return isSelected; }
    tgui::Button::Ptr getWidget() const { return button; }

private:
    tgui::Button::Ptr button;
    bool isHovered{false};
    bool clicked{false};
    bool wasPressed{false};
    bool isSelected{false};

    static const sf::Color defaultColor;
    static const sf::Color selectedColor;
};

} // namespace game
