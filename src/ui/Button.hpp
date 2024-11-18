#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <spdlog/spdlog.h>

namespace game {

class Button {
public:
    Button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size);
    void handleInput(const sf::Vector2f& mousePos);
    void setCallback(std::function<void()> callback);
    void render(sf::RenderWindow& window);

private:
    void handleClick(const sf::Vector2f& mousePos);
    void handleHover(const sf::Vector2f& mousePos);
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    void update(float deltaTime);

    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    std::function<void()> callback;
    bool isHovered;
    sf::Color defaultColor{sf::Color(100, 100, 100)};
    sf::Color hoverColor{sf::Color(150, 150, 150)};
};

} // namespace game
