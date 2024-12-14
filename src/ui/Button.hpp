#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <spdlog/spdlog.h>
#include "../core/ResourceManager.hpp"
#include "../input/InputManager.hpp"


namespace game {

class Button {
public:
    Button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size, const core::ResourceManager& resourceManager);
    void update(InputManager& inputManager);
    bool isClicked() const { return clicked; }
    void render(sf::RenderWindow& window);
    void setSelected(bool selected) { isSelected = selected; }
    bool getSelected() const { return isSelected; }

private:
    bool isMouseOver(const sf::Vector2f& mousePos) const;

    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    bool isHovered;
    bool clicked;
    bool wasPressed;  // Track previous frame's press state
    bool isSelected{false};  // Track if button is selected via keyboard/controller
    sf::Color defaultColor{sf::Color(100, 100, 100)};
    sf::Color hoverColor{sf::Color(150, 150, 150)};
    sf::Color selectedColor{sf::Color(200, 200, 200)};
};

} // namespace game
