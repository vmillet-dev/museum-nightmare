#include "Widget.hpp"

namespace game {

Widget::Widget(const sf::Vector2f& position, const sf::Vector2f& size)
    : position(position), size(size) {}

void Widget::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

void Widget::setSize(const sf::Vector2f& newSize) {
    size = newSize;
}

bool Widget::isMouseOver(const sf::Vector2f& mousePos) const {
    return mousePos.x >= position.x - size.x / 2 &&
           mousePos.x <= position.x + size.x / 2 &&
           mousePos.y >= position.y - size.y / 2 &&
           mousePos.y <= position.y + size.y / 2;
}

} // namespace game
