#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <spdlog/spdlog.h>

namespace game {

class Button {
public:
    Button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size) {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color(100, 100, 100));
        shape.setOutlineThickness(2.0f);
        shape.setOutlineColor(sf::Color::White);

        if (!font.loadFromFile("resources/fonts/arial.ttf")) {
            spdlog::error("Failed to load font");
        }

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(24);
        label.setFillColor(sf::Color::White);

        // Center text in button
        sf::FloatRect textBounds = label.getLocalBounds();
        label.setPosition(
            position.x + (size.x - textBounds.width) / 2.0f,
            position.y + (size.y - textBounds.height) / 2.0f
        );
    }

    void setCallback(std::function<void()> callback) {
        onClick = std::move(callback);
    }

    void handleInput(const sf::Vector2i& mousePos) {
        if (shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            shape.setFillColor(sf::Color(150, 150, 150));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (onClick) {
                    spdlog::debug("Button clicked: {}", label.getString().toAnsiString());
                    onClick();
                }
            }
        } else {
            shape.setFillColor(sf::Color(100, 100, 100));
        }
    }

    void render(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(label);
    }

private:
    sf::RectangleShape shape;
    sf::Text label;
    sf::Font font;
    std::function<void()> onClick;
};

} // namespace game
