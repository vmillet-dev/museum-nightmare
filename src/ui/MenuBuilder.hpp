#pragma once
#include "Button.hpp"
#include "../core/Game.hpp"
#include "../input/InputManager.hpp"
#include <vector>
#include <memory>
#include <functional>
#include <spdlog/spdlog.h>

namespace game {

class MenuBuilder {
public:
    explicit MenuBuilder(Game& game);

    // Adds a button to the menu with automatic vertical spacing and click handler
    MenuBuilder& addButton(const std::string& text, float centerX, float startY,
                         std::function<void()> onClick,
                         const sf::Vector2f& size = sf::Vector2f(200, 50));

    // Adds a button at a specific position with click handler
    MenuBuilder& addButtonAt(const std::string& text, const sf::Vector2f& position,
                           std::function<void()> onClick,
                           const sf::Vector2f& size = sf::Vector2f(200, 50));

    // Updates all buttons and handles navigation
    void update(InputManager& inputManager);

    // Get the built menu
    std::vector<std::unique_ptr<Button>> build();

    // Set vertical spacing between buttons
    MenuBuilder& setSpacing(float spacing) { buttonSpacing = spacing; return *this; }

    // Get current selected button index
    size_t getSelectedIndex() const { return selectedButtonIndex; }

private:
    Game& game;
    std::vector<std::unique_ptr<Button>> buttons;
    float buttonSpacing{70.0f};
    float currentY{0.0f};
    size_t selectedButtonIndex{0};
};

} // namespace game
