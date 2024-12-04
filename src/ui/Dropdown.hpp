#pragma once
#include "Widget.hpp"
#include "WidgetStyle.hpp"
#include <vector>
#include <string>

namespace game {

class Dropdown : public Widget {
public:
    Dropdown(const std::string& label, const sf::Vector2f& position, const sf::Vector2f& size,
            const std::vector<std::string>& options, const WidgetStyle& style = WidgetStyle::createDefault());

    void update(InputManager& inputManager) override;
    void render(sf::RenderWindow& window) override;

    void setOptions(const std::vector<std::string>& options);
    void setSelectedIndex(size_t index);
    size_t getSelectedIndex() const { return selectedIndex_; }
    const std::string& getSelectedOption() const { return options_[selectedIndex_]; }
    bool isExpanded() const { return isExpanded_; }

private:
    sf::RectangleShape mainBox_;
    sf::RectangleShape dropdownBox_;
    sf::Text labelText_;
    sf::Text selectedText_;
    sf::Font font_;
    WidgetStyle style_;

    std::vector<std::string> options_;
    size_t selectedIndex_ = 0;
    bool isExpanded_ = false;

    void updateTextPositions();
    size_t getHoveredOptionIndex(const sf::Vector2f& mousePos) const;
    void toggleExpanded();
};

} // namespace game
