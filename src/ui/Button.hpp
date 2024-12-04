#pragma once
#include "Widget.hpp"
#include "WidgetStyle.hpp"
#include <string>
#include <spdlog/spdlog.h>

namespace game {

class Button : public Widget {
public:
    Button(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size, const WidgetStyle& style = WidgetStyle::createDefault());
    void update(InputManager& inputManager) override;
    void render(sf::RenderWindow& window) override;

    void setText(const std::string& text);
    std::string getText() const { return text_.getString(); }  // Return by value instead of reference

private:
    sf::RectangleShape shape_;
    sf::Text text_;
    sf::Font font_;
    WidgetStyle style_;

    void updateTextPosition();
};

} // namespace game
