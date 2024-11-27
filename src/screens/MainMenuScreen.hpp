#pragma once
#include "Screen.hpp"
#include "../ui/Button.hpp"
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

private:
    std::vector<std::unique_ptr<Button>> buttons;
    bool shouldQuit = false;
    Game& game;
    size_t selectedButtonIndex;  // Track currently selected button
};

} // namespace game
