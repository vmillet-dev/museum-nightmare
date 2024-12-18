#pragma once
#include "Screen.hpp"
#include "../ui/TGUIButtonWrapper.hpp"
#include "ScreenManager.hpp"
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace game {

class Game;  // Forward declaration
class MainMenuScreen;  // Forward declaration

class PauseScreen : public Screen {
public:
    PauseScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event) { gui.handleEvent(event); }

private:
    tgui::Gui gui;
    std::vector<std::unique_ptr<TGUIButtonWrapper>> buttons;
    sf::Font font;
    sf::Text pauseText;
    Game& game;
    size_t selectedButtonIndex{0};  // Track currently selected button
};

} // namespace game
