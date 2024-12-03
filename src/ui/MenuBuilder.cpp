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

    buttons.push_back(std::make_unique<Button>(text, sf::Vector2f(centerX, currentY), size));
    clickHandlers.push_back(std::move(onClick));

    // Set initial selection for first button
    if (buttons.size() == 1) {
        buttons[0]->setSelected(true);
    }

    spdlog::debug("Added button '{}' at position ({}, {})", text, centerX, currentY);
    return *this;
}

MenuBuilder& MenuBuilder::addButtonAt(const std::string& text, const sf::Vector2f& position,
                                    std::function<void()> onClick, const sf::Vector2f& size) {
    buttons.push_back(std::make_unique<Button>(text, position, size));
    clickHandlers.push_back(std::move(onClick));

    // Set initial selection for first button
    if (buttons.size() == 1) {
        buttons[0]->setSelected(true);
    }

    spdlog::debug("Added button '{}' at specific position ({}, {})", text, position.x, position.y);
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

    // Update all buttons and handle clicks
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i]->update(inputManager);
        if (buttons[i]->isClicked() && i < clickHandlers.size()) {
            clickHandlers[i]();
        }
    }
}

std::vector<std::unique_ptr<Button>> MenuBuilder::build() {
    spdlog::debug("Building menu with {} buttons", buttons.size());
    return std::move(buttons);
}

} // namespace game
