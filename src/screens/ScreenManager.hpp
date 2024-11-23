#pragma once
#include <memory>
#include <map>
#include "Screen.hpp"
#include "../core/GameState.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Game;  // Forward declaration

class ScreenManager {
public:
    explicit ScreenManager(Game& game);

    void setState(GameState newState);
    GameState getCurrentState() const { return currentState; }
    Screen* getCurrentScreen() const { return currentScreen; }

    template<typename T>
    void registerScreen(GameState state) {
        screens[state] = std::make_unique<T>(game);
    }

    void handleInput(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

private:
    Game& game;
    GameState currentState = GameState::MainMenu;
    Screen* currentScreen = nullptr;
    std::map<GameState, std::unique_ptr<Screen>> screens;
};

} // namespace game
