#pragma once
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "Screen.hpp"
#include "ScreenManager.hpp"
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>

namespace game {

class Game;  // Forward declaration
class GameScreen;  // Forward declaration

class MainMenuScreen : public Screen {
public:
    MainMenuScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool shouldExitGame() const { return shouldQuit; }
    void handleEvent(const sf::Event& event) { gui.handleEvent(event); }

private:
    tgui::Gui gui;
    std::vector<tgui::Button::Ptr> m_buttons;
    bool shouldQuit = false;
    Game& game;
    size_t selectedButtonIndex;  // Track currently selected button
};

} // namespace game
