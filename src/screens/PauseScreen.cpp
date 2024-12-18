#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : game(game), gui(game.getWindow()) {
    spdlog::info("Initializing pause screen");

    // Create buttons
    auto resumeButton = tgui::Button::create("Resume");
    resumeButton->setPosition({"400", "250"});
    resumeButton->setSize({"200", "50"});

    auto mainMenuButton = tgui::Button::create("Main Menu");
    mainMenuButton->setPosition({"400", "350"});
    mainMenuButton->setSize({"200", "50"});

    // Style buttons
    for (auto& button : {resumeButton, mainMenuButton}) {
        button->getRenderer()->setBackgroundColor(tgui::Color(100, 100, 100));
        button->getRenderer()->setBorderColor(tgui::Color::White);
        button->getRenderer()->setTextColor(tgui::Color::White);
        button->getRenderer()->setBackgroundColorHover(tgui::Color(150, 150, 150));
        button->getRenderer()->setBackgroundColorFocused(tgui::Color(200, 200, 200));
    }

    // Set up button callbacks
    resumeButton->onPress([this, &game]() {
        spdlog::info("Resuming game");
        game.getScreenManager().setState(GameState::Playing);
    });

    mainMenuButton->onPress([this, &game]() {
        spdlog::info("Returning to main menu");
        game.getScreenManager().setState(GameState::MainMenu);
    });

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
    pauseText.setPosition(400, 150);

    // Add buttons to vector and GUI
    buttons = {resumeButton, mainMenuButton};
    for (auto& button : buttons) {
        gui.add(button);
    }
    buttons[selectedButtonIndex]->setFocused(true);
}

void PauseScreen::update(float deltaTime) {
    auto& inputManager = game.getInputManager();

    // Handle button navigation
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        buttons[selectedButtonIndex]->setFocused(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        buttons[selectedButtonIndex]->setFocused(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
    }
}

void PauseScreen::render(sf::RenderWindow& window) {
    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Draw pause text
    window.draw(pauseText);

    // Draw GUI elements
    gui.draw();
}

} // namespace game
