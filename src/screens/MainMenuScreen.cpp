#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>
#include "../ui/GuiBuilder.hpp"

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), selectedButtonIndex(0) {
    gui.setTarget(game.getWindow());

    GuiBuilder(gui)
        .addVerticalLayout("mainLayout")
        .addLabel("Game Menu")
        .addButton("Play", []() { std::cout << "Play button" << std::endl; })
            .setTextSize(20)
            .endButton()
        .addButton("Quit", []() { std::cout << "Quit button" << std::endl; })
            .setTextSize(20)
            .endButton()
        .build();

    // Store references to buttons for navigation
    //m_buttons = {
    //    gui.get<tgui::Button>("Play"),
    //    gui.get<tgui::Button>("Quit")
    //};
    //
    //// Initialize first button as selected
    //if (!m_buttons.empty() && m_buttons[0]) {
    //    m_buttons[0]->setFocused(true);
    //}
}

void MainMenuScreen::update(float deltaTime) {
    auto& inputManager = game.getInputManager();

    // Handle button navigation
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        if (!m_buttons.empty() && m_buttons[selectedButtonIndex]) {
            m_buttons[selectedButtonIndex]->setFocused(false);
        }
        selectedButtonIndex = (selectedButtonIndex + 1) % m_buttons.size();
        if (m_buttons[selectedButtonIndex]) {
            m_buttons[selectedButtonIndex]->setFocused(true);
        }
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        if (!m_buttons.empty() && m_buttons[selectedButtonIndex]) {
            m_buttons[selectedButtonIndex]->setFocused(false);
        }
        selectedButtonIndex = (selectedButtonIndex - 1 + m_buttons.size()) % m_buttons.size();
        if (m_buttons[selectedButtonIndex]) {
            m_buttons[selectedButtonIndex]->setFocused(true);
        }
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    gui.draw();
}

} // namespace game
