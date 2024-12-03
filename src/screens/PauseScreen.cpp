#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : game(game) {
    spdlog::info("Initializing pause screen");

    // Create resume button
    resumeButton = std::make_unique<Button>(
        "Resume",
        sf::Vector2f(400, 250),
        sf::Vector2f(200, 50),
        game.getResourceManager()  // Pass ResourceManager reference
    );
    resumeButton->setCallback([this]() {
        spdlog::info("Resuming game");
        ScreenManager::getInstance().popScreen();
    });

    // Create main menu button
    mainMenuButton = std::make_unique<Button>(
        "Main Menu",
        sf::Vector2f(400, 350),
        sf::Vector2f(200, 50),
        game.getResourceManager()  // Pass ResourceManager reference
    );
    mainMenuButton->setCallback([this, &game]() {
        spdlog::info("Returning to main menu");
        // Pop both pause screen and game screen
        ScreenManager::getInstance().popScreen();
        ScreenManager::getInstance().popScreen();
        // Push new main menu screen
        ScreenManager::getInstance().pushScreen(std::make_unique<MainMenuScreen>(game));
    });

    // Load font
    if (!font.loadFromFile("assets/arial.ttf")) {
        spdlog::error("Failed to load font!");
    }

    // Create pause text
    pauseText.setFont(font);
    pauseText.setString("Paused");
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(sf::Color::White);

    // Center the pause text
    sf::FloatRect textBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    pauseText.setPosition(400, 150);
}

void PauseScreen::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed ||
        event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos;
        if (event.type == sf::Event::MouseButtonPressed) {
            mousePos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        } else {
            mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
        }
        sf::Vector2f mousePosFloat(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        resumeButton->handleInput(mousePosFloat);
        mainMenuButton->handleInput(mousePosFloat);
    }
}

void PauseScreen::update(float deltaTime) {
    // No update needed for buttons in our implementation
}

void PauseScreen::render(sf::RenderWindow& window) {
    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Draw pause menu elements
    window.draw(pauseText);
    resumeButton->render(window);
    mainMenuButton->render(window);
}

} // namespace game
