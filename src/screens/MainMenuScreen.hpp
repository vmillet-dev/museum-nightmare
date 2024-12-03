#pragma once
#include "Screen.hpp"
#include "../ui/MenuBuilder.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Game;  // Forward declaration
class GameScreen;  // Forward declaration

class MainMenuScreen : public Screen {
public:
    explicit MainMenuScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool shouldExitGame() const { return shouldQuit; }

private:
    MenuBuilder menuBuilder_;
    Button* playButton_;
    Button* settingsButton_;
    Button* testButton_;     // Add test button
    Button* quitButton_;
    bool shouldQuit = false;
};

} // namespace game
