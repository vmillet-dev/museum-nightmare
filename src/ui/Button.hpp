#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <spdlog/spdlog.h>
#include "../input/InputManager.hpp"

namespace game {

class Button {
public:
    Button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size);
    void handleInput(const sf::Vector2f& mousePos, const InputManager& inputManager);
    void render(sf::RenderWindow& window);
    bool isPressed() const { return isActive; }
    const std::string& getText() const { return buttonText; }

private:
    void handleHover(const sf::Vector2f& mousePos);
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    void update(float deltaTime);

    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    std::string buttonText;
    bool isHovered;
    bool isActive;
    sf::Color defaultColor{sf::Color(100, 100, 100)};
    sf::Color hoverColor{sf::Color(150, 150, 150)};
};

} // namespace game
