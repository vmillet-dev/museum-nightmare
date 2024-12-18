#pragma once
#include <SFML/Graphics.hpp>
#include "./GameState.hpp"
#include "../screens/ScreenManager.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include "../screens/PauseScreen.hpp"
#include "../input/InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include "ResourceManager.hpp"
#include "SoundManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Game {
public:
    Game();
    void run();
    void quit();
    sf::RenderWindow& getWindow() { return window;  }
    InputManager& getInputManager() { return inputManager; }
    ResourceManager& getResourceManager() { return resourceManager; }
    SoundManager& getSoundManager() { return soundManager; }
    ScreenManager& getScreenManager() { return screenManager; }


private:
    void handleEvent(const sf::Event& event);
    void handleEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow window;

    ResourceManager resourceManager;
    SoundManager soundManager{resourceManager};  // Initialize with resourceManager
    InputManager inputManager{ window };  // Initialize with window reference
    ScreenManager screenManager{ *this };
};

} // namespace game
