#include "MenuBuilder.hpp"

namespace game {

MenuBuilder::MenuBuilder(Game& game) : game(game), currentY(0.0f), selectedButtonIndex(0) {}

MenuBuilder& MenuBuilder::addButton(const std::string& text, float centerX, float startY,
                                  std::function<void()> onClick, const sf::Vector2f& size) {
    if (buttons.empty()) {
        currentY = startY;
    } else {
        currentY += buttonSpacing;
    }

    auto button = std::make_unique<Button>(text, sf::Vector2f(centerX, currentY), size);
    button->setOnClick(onClick);  // Set the onClick handler directly on the button

    // Set initial selection for first button
    if (buttons.empty()) {
        button->setSelected(true);
    }

    spdlog::debug("Added button '{}' at position ({}, {})", text, centerX, currentY);
    buttons.push_back(std::move(button));
    return *this;
}

MenuBuilder& MenuBuilder::addButtonAt(const std::string& text, const sf::Vector2f& position,
                                    std::function<void()> onClick, const sf::Vector2f& size) {
    auto button = std::make_unique<Button>(text, position, size);
    button->setOnClick(onClick);  // Set the onClick handler directly on the button

    // Set initial selection for first button
    if (buttons.empty()) {
        button->setSelected(true);
    }

    spdlog::debug("Added button '{}' at specific position ({}, {})", text, position.x, position.y);
    buttons.push_back(std::move(button));
    return *this;
}

void MenuBuilder::update(InputManager& inputManager) {
    // Handle navigation
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
    }

    // Update all buttons
    for (auto& button : buttons) {
        button->update(inputManager);
    }
}

std::vector<std::unique_ptr<Button>> MenuBuilder::build() {
    spdlog::debug("Building menu with {} buttons", buttons.size());
    return std::move(buttons);
}

} // namespace game
