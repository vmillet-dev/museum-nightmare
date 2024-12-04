#pragma once
#include "Widget.hpp"
#include "Button.hpp"
#include "Dropdown.hpp"
#include <memory>
#include <vector>
#include <functional>

namespace game {

class Game;  // Forward declaration

class MenuBuilder {
public:
    using ButtonCallback = std::function<void()>;

    explicit MenuBuilder(Game& game);

    // Method chaining interface
    MenuBuilder& setSpacing(float spacing) {
        spacing_ = spacing;
        return *this;
    }

    MenuBuilder& addButton(const std::string& text, float x, float y, ButtonCallback callback);
    Dropdown* addDropdown(const std::string& label, const sf::Vector2f& position, const sf::Vector2f& size,
                         const std::vector<std::string>& options,
                         const WidgetStyle& style = WidgetStyle::createDefault());

    // Build and get results
    std::vector<Button*> build();
    size_t getSelectedIndex() const { return selectedIndex_; }

    // Widget management
    void update(InputManager& inputManager);
    void render(sf::RenderWindow& window);

    // Navigation
    void selectNext();
    void selectPrevious();
    Widget* getSelectedWidget();

private:
    Game& game_;
    std::vector<std::unique_ptr<Widget>> widgets_;
    std::vector<ButtonCallback> callbacks_;
    size_t selectedIndex_ = 0;
    float spacing_ = 20.f;
    float nextY_ = 0.f;
    float startX_ = 0.f;

    void updateSelection();
    void executeCallback(size_t index);
};

} // namespace game
