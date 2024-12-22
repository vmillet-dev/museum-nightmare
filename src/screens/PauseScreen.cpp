#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : game(game), selectedButtonIndex(0) {
    spdlog::info("Initializing pause screen");
    gui.setTarget(game.getWindow());

    // Initialize MenuBuilder and create buttons
    MenuBuilder menuBuilder(&gui);

    // Set up menu layout first
    menuBuilder
    .setLayout(game::LayoutType::Vertical)
    .setSpacing(20.f)
    .setPadding(50.f)
    .setTheme("assets/themes/dark.theme")
    .setResponsive(true);

    // Add pause title
    menuBuilder.addLabel("Paused")
        .setSize("400", "50")
        .build();

    // Add buttons
    menuBuilder.addButton("Resume", [this, &game]{
        spdlog::info("Resuming game");
        game.getScreenManager().setState(GameState::Playing);
    })
    .setSize("200", "50")
    .build();

    menuBuilder.addButton("Main Menu", [this, &game]{
        spdlog::info("Returning to main menu");
        game.getScreenManager().setState(GameState::MainMenu);
    })
    .setSize("200", "50")
    .build();

    menuBuilder.build();

    // Store references to buttons for navigation
    m_buttons = {
        gui.get<tgui::Button>("Resume"),
        gui.get<tgui::Button>("Main Menu")
    };

    // Initialize first button as selected
    if (!m_buttons.empty() && m_buttons[0]) {
        m_buttons[0]->setFocused(true);
    }

    // Load font and setup pause text
    if (!font.loadFromFile("assets/arial.ttf")) {
        spdlog::error("Failed to load font!");
    }

    pauseText.setFont(font);
    pauseText.setString("Paused");
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(sf::Color::White);

    // Center the pause text
    sf::FloatRect textBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(textBounds.width / 2, textBounds.height / 2);

    // Position the pause text above the buttons
    const sf::Vector2u windowSize = game.getWindow().getSize();
    const float buttonHeight = 50.f;
    const float startY = windowSize.y / 2.f - buttonHeight;
    pauseText.setPosition(windowSize.x / 2.f, startY - buttonHeight);
}

void PauseScreen::update(float deltaTime) {
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
        spdlog::debug("Pause menu: Selected button {}", selectedButtonIndex);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        if (!m_buttons.empty() && m_buttons[selectedButtonIndex]) {
            m_buttons[selectedButtonIndex]->setFocused(false);
        }
        selectedButtonIndex = (selectedButtonIndex - 1 + m_buttons.size()) % m_buttons.size();
        if (m_buttons[selectedButtonIndex]) {
            m_buttons[selectedButtonIndex]->setFocused(true);
        }
        spdlog::debug("Pause menu: Selected button {}", selectedButtonIndex);
    }
}

void PauseScreen::render(sf::RenderWindow& window) {
    // Store current view
    sf::View currentView = window.getView();

    // Reset to default view for TGUI rendering
    window.setView(window.getDefaultView());

    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Draw pause text
    window.draw(pauseText);

    // Draw GUI elements
    gui.draw();

    // Restore original view
    window.setView(currentView);
}

} // namespace game
