#pragma once
#include "Screen.hpp"
#include "../ui/Button.hpp"
#include "ScreenManager.hpp"
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>

namespace game {

class Game;  // Forward declaration
class GameScreen;  // Forward declaration

class MainMenuScreen : public Screen {
public:
    MainMenuScreen(Game& game) : game(game) {
        buttons.push_back(std::make_unique<Button>("Play", sf::Vector2f(300, 200), sf::Vector2f(200, 50)));
        buttons.push_back(std::make_unique<Button>("Quit", sf::Vector2f(300, 300), sf::Vector2f(200, 50)));

        buttons[0]->setCallback([&]() {
            spdlog::info("Starting game");
            auto gameScreen = std::make_unique<GameScreen>(game);
            ScreenManager::getInstance().pushScreen(std::unique_ptr<Screen>(gameScreen.release()));
        });

        buttons[1]->setCallback([this]() {
            spdlog::info("Quitting game");
            shouldQuit = true;
        });
    }

    void handleInput(const sf::Event& event) override {
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
            for (auto& button : buttons) {
                button->handleInput(mousePos);
            }
        }
    }

    void update(float deltaTime) override {}

    void render(sf::RenderWindow& window) override {
        for (auto& button : buttons) {
            button->render(window);
        }
    }

    bool shouldExitGame() const { return shouldQuit; }

private:
    std::vector<std::unique_ptr<Button>> buttons;
    bool shouldQuit = false;
    Game& game;
};

} // namespace game
