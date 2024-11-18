#pragma once
#include <SFML/Graphics.hpp>
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
    sf::RenderWindow& getWindow();

private:
    void handleEvent(const sf::Event& event);
    void handleEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow window;
};

} // namespace game
