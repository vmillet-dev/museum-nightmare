#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <spdlog/spdlog.h>
#include "../input/InputManager.hpp"

namespace game {

class Button {
public:
    Button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size);
    void update(InputManager& inputManager);
    bool isClicked() const { return clicked; }
    void render(sf::RenderWindow& window);

private:
    bool isMouseOver(const sf::Vector2f& mousePos) const;

    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    bool isHovered;
    bool clicked;
    bool wasPressed;  // Track previous frame's press state
    sf::Color defaultColor{sf::Color(100, 100, 100)};
    sf::Color hoverColor{sf::Color(150, 150, 150)};
};

} // namespace game
