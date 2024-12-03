#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
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
    void setSelected(bool selected) { isSelected = selected; updateVisualState(); }
    bool getSelected() const { return isSelected; }

    // Add getters for position to help with navigation
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }

private:
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    void updateVisualState();
    void updateEffects(float deltaTime);

    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    bool isHovered;
    bool clicked;
    bool wasPressed;
    bool isSelected{false};

    // Enhanced visual states
    float pulseEffect{0.0f};
    float transitionEffect{0.0f};
    sf::Clock effectClock;

    // Enhanced colors for better visual feedback
    sf::Color defaultColor{sf::Color(80, 80, 80)};
    sf::Color hoverColor{sf::Color(120, 120, 120)};
    sf::Color selectedColor{sf::Color(160, 160, 160)};
    sf::Color pressedColor{sf::Color(200, 200, 200)};
    sf::Color outlineDefaultColor{sf::Color::White};
    sf::Color outlineSelectedColor{sf::Color(255, 215, 0)}; // Gold color for selection
};

} // namespace game
