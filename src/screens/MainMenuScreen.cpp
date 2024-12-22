#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), selectedButtonIndex(0) {
    gui.setTarget(game.getWindow());

    MenuBuilder menuBuilder(&gui);

    // Create buttons with WidgetBuilder pattern
    menuBuilder
    .setLayout(game::LayoutType::Vertical)
    .setSpacing(20.f)
    .setPadding(50.f)
    .setTheme("assets/themes/dark.theme")
    .setResponsive(true);

    // Add title
    menuBuilder.addLabel("Museum Nightmare")
        .setSize("400", "50")
        .build();

    // Add buttons
    menuBuilder.addButton("Play", [this, &game]{
        spdlog::info("Starting game");
        game.getScreenManager().setState(GameState::Playing);
    })
    .setSize("200", "50")
    .build();

    menuBuilder.addButton("Quit", [this, &game]{
        spdlog::info("Quitting game");
        game.getScreenManager().setState(GameState::Quit);
    })
    .setSize("200", "50")
    .build();

    menuBuilder.build();

    // Store references to buttons for navigation
    m_buttons = {
        gui.get<tgui::Button>("Play"),
        gui.get<tgui::Button>("Quit")
    };

    // Initialize first button as selected
    if (!m_buttons.empty() && m_buttons[0]) {
        m_buttons[0]->setFocused(true);
    }
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
