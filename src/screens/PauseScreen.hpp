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

class PauseScreen : public Screen {
public:
    PauseScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(Game& game);
    void handleEvent(const sf::Event& event);

private:
    Game& game;
    MenuBuilder m_menuBuilder;
};

} // namespace game
