#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "./GameState.hpp"
#include "../screens/ScreenManager.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include "../screens/PauseScreen.hpp"
#include "../input/InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Game {
public:
    Game();
    void run();
    void quit();
    sf::RenderWindow& getWindow() { return window; }
    InputManager& getInputManager() { return inputManager; }
    ScreenManager& getScreenManager() { return screenManager; }
    tgui::Gui& getGui() { return gui; }

private:
    void handleEvent(const sf::Event& event);
    void handleEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow window;
    tgui::Gui gui{window};
    InputManager inputManager{window};
    ScreenManager screenManager{*this};
};

} // namespace game
