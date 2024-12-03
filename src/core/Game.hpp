#pragma once
#include <SFML/Graphics.hpp>
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
    sf::RenderWindow& getWindow();
    InputManager& getInputManager() { return inputManager; }
    core::ResourceManager& getResourceManager() { return resourceManager; }
    core::SoundManager& getSoundManager() { return soundManager; }

private:
    void handleEvent(const sf::Event& event);
    void handleEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow window;
    InputManager inputManager;
    core::ResourceManager resourceManager;
    core::SoundManager soundManager{resourceManager};  // Initialize with resourceManager
};

} // namespace game
