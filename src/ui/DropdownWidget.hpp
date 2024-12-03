#pragma once
#include "Widget.hpp"
#include <vector>
#include <string>
#include <functional>

namespace game {

class DropdownWidget : public Widget {
public:
    DropdownWidget(const sf::Vector2f& position, const sf::Vector2f& size,
                  const std::vector<std::string>& options);

    void update(InputManager& inputManager) override;
    void render(sf::RenderWindow& window) override;

    void setOnSelectionChanged(std::function<void(size_t)> callback) { onSelectionChanged = callback; }
    size_t getCurrentSelection() const { return currentSelection; }
    void setCurrentSelection(size_t index);

private:
    void updateVisualState();
    void toggleDropdown();
    bool isOptionHovered(size_t index, const sf::Vector2f& mousePos) const;

    std::vector<std::string> options;
    size_t currentSelection{0};
    bool isOpen{false};

    sf::RectangleShape mainBox;
    sf::RectangleShape dropdownBox;
    sf::Text currentText;
    std::vector<sf::Text> optionTexts;
    sf::Font font;

    // Visual properties
    sf::Color backgroundColor{80, 80, 80};
    sf::Color hoverColor{120, 120, 120};
    sf::Color selectedColor{160, 160, 160};
    sf::Color textColor{255, 255, 255};
    float optionHeight{30.0f};

    std::function<void(size_t)> onSelectionChanged;
    size_t hoveredOption{static_cast<size_t>(-1)};
};

} // namespace game
