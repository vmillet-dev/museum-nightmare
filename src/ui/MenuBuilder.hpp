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

    // Button creation with method chaining and direct access
    Button* addButton(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size,
                      const WidgetStyle& style = WidgetStyle::createDefault());

    // Dropdown creation with method chaining and direct access
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

    // Widget access
    template<typename T>
    T* getWidget(size_t index) {
        if (index < widgets_.size()) {
            return dynamic_cast<T*>(widgets_[index].get());
        }
        return nullptr;
    }

    // Layout helpers
    void centerHorizontally(float y, float spacing = 20.f);
    void alignVertically(float x, float spacing = 20.f);

private:
    std::vector<std::unique_ptr<Widget>> widgets_;
    size_t selectedIndex_ = 0;

    void updateSelection();
};

} // namespace game
