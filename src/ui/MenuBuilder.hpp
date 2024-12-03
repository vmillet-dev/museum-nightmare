#pragma once
#include "Widget.hpp"
#include "Button.hpp"
#include "Dropdown.hpp"
#include <memory>
#include <vector>
#include <functional>

namespace game {

class MenuBuilder {
public:
    MenuBuilder() = default;

    // Button creation
    Button* addButton(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size,
                     const WidgetStyle& style = WidgetStyle::createDefault());

    // Dropdown creation
    Dropdown* addDropdown(const std::string& label, const sf::Vector2f& position, const sf::Vector2f& size,
                         const std::vector<std::string>& options,
                         const WidgetStyle& style = WidgetStyle::createDefault());

    // Widget management
    void update(InputManager& inputManager);
    void render(sf::RenderWindow& window);

    // Navigation
    void selectNext();
    void selectPrevious();
    Widget* getSelectedWidget();
    const std::vector<std::unique_ptr<Widget>>& getWidgets() const { return widgets_; }

private:
    std::vector<std::unique_ptr<Widget>> widgets_;
    size_t selectedIndex_ = 0;

    void updateSelection();
};

} // namespace game
