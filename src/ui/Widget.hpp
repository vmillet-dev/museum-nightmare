#pragma once
#include <SFML/Graphics.hpp>
#include "../input/InputManager.hpp"

namespace game {

class Widget {
public:
    Widget() = default;
    virtual ~Widget() = default;

    // Core widget functionality
    virtual void update(InputManager& inputManager) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    // Common widget properties
    virtual void setPosition(const sf::Vector2f& position) { position_ = position; }
    virtual sf::Vector2f getPosition() const { return position_; }
    virtual void setSize(const sf::Vector2f& size) { size_ = size; }
    virtual sf::Vector2f getSize() const { return size_; }

    // Selection and focus handling
    virtual void setSelected(bool selected) { isSelected_ = selected; }
    virtual bool isSelected() const { return isSelected_; }
    virtual bool isFocusable() const { return focusable_; }
    virtual void setFocusable(bool focusable) { focusable_ = focusable; }

    // Common state queries
    virtual bool isHovered() const { return isHovered_; }
    virtual bool isClicked() const { return clicked_; }

protected:
    // Common widget state
    sf::Vector2f position_;
    sf::Vector2f size_;
    bool isSelected_ = false;
    bool isHovered_ = false;
    bool clicked_ = false;
    bool wasPressed_ = false;
    bool focusable_ = true;

    // Common colors for consistent theming
    sf::Color defaultColor{sf::Color(100, 100, 100)};
    sf::Color hoverColor{sf::Color(150, 150, 150)};
    sf::Color selectedColor{sf::Color(200, 200, 200)};
    sf::Color disabledColor{sf::Color(70, 70, 70)};

    // Utility functions for derived classes
    virtual bool isMouseOver(const sf::Vector2f& mousePos) const {
        sf::FloatRect bounds(position_, size_);
        return bounds.contains(mousePos);
    }
};

} // namespace game
