#pragma once
#include <SFML/Graphics.hpp>
#include "../input/InputManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Widget {
public:
    Widget(const sf::Vector2f& position, const sf::Vector2f& size);
    virtual ~Widget() = default;

    virtual void update(InputManager& inputManager) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    // Common widget functionality
    virtual void setPosition(const sf::Vector2f& pos);
    virtual void setSize(const sf::Vector2f& size);
    virtual bool isMouseOver(const sf::Vector2f& mousePos) const;

    // Getters
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    bool isSelected() const { return selected; }
    void setSelected(bool select) { selected = select; }

protected:
    sf::Vector2f position;
    sf::Vector2f size;
    bool selected{false};
    bool hovered{false};
};

} // namespace game
