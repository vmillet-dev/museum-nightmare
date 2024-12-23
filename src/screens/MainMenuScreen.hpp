#pragma once
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "Screen.hpp"
#include "../ui/MenuBuilder.hpp"
#include "ScreenManager.hpp"
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
    Game& game;
    MenuBuilder m_menuBuilder;
    bool shouldQuit = false;
};

} // namespace game
