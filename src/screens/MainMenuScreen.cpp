#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& gameRef) : game(gameRef) {
    buttons.push_back(std::make_unique<Button>("Play", sf::Vector2f(300, 200), sf::Vector2f(200, 50)));
    buttons.push_back(std::make_unique<Button>("Quit", sf::Vector2f(300, 300), sf::Vector2f(200, 50)));

    buttons[0]->setCallback([&]() {
        spdlog::info("Starting game");
        auto screen = std::make_unique<GameScreen>(game);
        ScreenManager::getInstance().pushScreen(std::move(screen));
    });

    buttons[1]->setCallback([this]() {
        spdlog::info("Quitting game");
        shouldQuit = true;
    });
}

void MainMenuScreen::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
        sf::Vector2f mousePosFloat(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        for (auto& button : buttons) {
            button->handleInput(mousePosFloat);
        }
    }
}

void MainMenuScreen::update([[maybe_unused]] float deltaTime) {}

void MainMenuScreen::render(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button->render(window);
    }
}

} // namespace game
