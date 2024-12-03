#pragma once
#include <SFML/Graphics.hpp>

namespace game {

struct WidgetStyle {
    sf::Color defaultColor{sf::Color(100, 100, 100)};
    sf::Color hoverColor{sf::Color(150, 150, 150)};
    sf::Color selectedColor{sf::Color(200, 200, 200)};
    sf::Color disabledColor{sf::Color(70, 70, 70)};
    sf::Color textColor{sf::Color::White};
    float outlineThickness{2.0f};
    sf::Color outlineColor{sf::Color::White};
    unsigned int characterSize{24};
    std::string fontPath{"assets/arial.ttf"};

    static WidgetStyle createDefault() {
        return WidgetStyle{};
    }
};

} // namespace game
