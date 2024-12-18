#pragma once
#include "Screen.hpp"
#include "../ui/TGUIButtonWrapper.hpp"
#include "ScreenManager.hpp"
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

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
    std::vector<std::unique_ptr<TGUIButtonWrapper>> buttons;
    bool shouldQuit = false;
    Game& game;
    size_t selectedButtonIndex;  // Track currently selected button
};

} // namespace game
