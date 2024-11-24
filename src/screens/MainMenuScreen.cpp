#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../input/devices/InputDevice.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), selectedButtonIndex(0) {
    buttons.push_back(std::make_unique<Button>("Play", sf::Vector2f(300, 200), sf::Vector2f(200, 50)));
    buttons.push_back(std::make_unique<Button>("Quit", sf::Vector2f(300, 300), sf::Vector2f(200, 50)));
    // Set initial button selection
    buttons[selectedButtonIndex]->setSelected(true);
}

void MainMenuScreen::handleInput(const sf::Event& event) {
    auto& inputManager = game.getInputManager();

    // Handle button navigation
    if (inputManager.getActionState(Action::MoveDown) == InputDevice::ActionState::JUST_PRESSED) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
        spdlog::debug("Menu: Selected button index {}", selectedButtonIndex);
    }
    if (inputManager.getActionState(Action::MoveUp) == InputDevice::ActionState::JUST_PRESSED) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
        spdlog::debug("Menu: Selected button index {}", selectedButtonIndex);
    }

    // Handle button activation
    if (inputManager.getActionState(Action::Confirm) == InputDevice::ActionState::JUST_PRESSED) {
        spdlog::debug("Menu: Confirming button {}", selectedButtonIndex);
        if (selectedButtonIndex == 0) {  // Play button
            spdlog::info("Starting game");
            game.getScreenManager().setState(GameState::Playing);
        } else if (selectedButtonIndex == 1) {  // Quit button
            spdlog::info("Quitting game");
            shouldQuit = true;
        }
    }

    // Update all buttons with input manager
    for (auto& button : buttons) {
        button->update(inputManager);
    }
}

void MainMenuScreen::update(float deltaTime) {}

void MainMenuScreen::render(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button->render(window);
    }
}

} // namespace game
