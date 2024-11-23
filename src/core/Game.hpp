#pragma once
#include <SFML/Graphics.hpp>
#include "../input/InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include "../screens/ScreenManager.hpp"
#include "StateManager.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include "../screens/PauseScreen.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Game {
public:
    Game();
    void run();
    void quit();
    sf::RenderWindow& getWindow();
    InputManager& getInputManager() { return inputManager; }
    ScreenManager& getScreenManager() { return screenManager; }
    StateManager& getStateManager() { return stateManager; }

private:
    void handleEvent(const sf::Event& event);
    void handleEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow window;
    InputManager inputManager;
    ScreenManager screenManager;
    StateManager stateManager;
};

} // namespace game
