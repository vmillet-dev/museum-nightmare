#pragma once
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "Screen.hpp"
#include "GameState.hpp"
#include <memory>
#include <spdlog/spdlog.h>

namespace game {

class Game;  // Forward declaration
class GameScreen;  // Forward declaration

class MainMenuScreen : public Screen {
public:
    MainMenuScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(Game& game);
    void handleEvent(const sf::Event& event);

private:
    bool shouldQuit = false;
};

} // namespace game
