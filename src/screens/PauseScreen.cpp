#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : game(game) {
    spdlog::info("Initializing pause screen");
    gui.setTarget(game.getWindow());

    // Initialize MenuBuilder and create buttons
    m_menuBuilder = std::make_shared<MenuBuilder>(&gui);
    m_container = m_menuBuilder
        ->addButton("resumeButton", "Resume")
        ->addButton("mainMenuButton", "Main Menu")
        ->setVerticalLayout()
        ->setSpacing(20.f)
        ->setPadding(50.f)
        ->onClick("resumeButton", [this, &game]{
            spdlog::info("Resuming game");
            game.getScreenManager().setState(GameState::Playing);
        })
        ->onClick("mainMenuButton", [this, &game]{
            spdlog::info("Returning to main menu");
            game.getScreenManager().setState(GameState::MainMenu);
        })
        ->setTheme("themes/dark.theme")
        ->setResponsive(true)
        ->build();

    gui.add(m_container);

    // Initialize first button as selected
    if (auto button = m_container->get<tgui::Button>("resumeButton")) {
        button->setFocused(true);
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
        if (auto currentButton = m_container->get<tgui::Button>(selectedButtonIndex == 0 ? "resumeButton" : "mainMenuButton")) {
            currentButton->setFocused(false);
        }
        selectedButtonIndex = (selectedButtonIndex + 1) % 2;
        if (auto nextButton = m_container->get<tgui::Button>(selectedButtonIndex == 0 ? "resumeButton" : "mainMenuButton")) {
            nextButton->setFocused(true);
        }
        spdlog::debug("Pause menu: Selected button {}", selectedButtonIndex);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        if (auto currentButton = m_container->get<tgui::Button>(selectedButtonIndex == 0 ? "resumeButton" : "mainMenuButton")) {
            currentButton->setFocused(false);
        }
        selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2;
        if (auto nextButton = m_container->get<tgui::Button>(selectedButtonIndex == 0 ? "resumeButton" : "mainMenuButton")) {
            nextButton->setFocused(true);
        }
        spdlog::debug("Pause menu: Selected button {}", selectedButtonIndex);
    }
}

void PauseScreen::render(sf::RenderWindow& window) {
    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Draw pause text
    window.draw(pauseText);

    // Draw GUI elements
    gui.draw();
}

} // namespace game
