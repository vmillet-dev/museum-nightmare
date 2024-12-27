#pragma once
#include "Screen.hpp"
#include "../ui/Button.hpp"
#include "ScreenManager.hpp"
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

namespace game {

class Game;  // Forward declaration
class MainMenuScreen;  // Forward declaration

class PauseScreen : public Screen {
public:
    PauseScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    Game& game;
    sf::Font font;  // Font must be declared before Text that uses it
    std::unique_ptr<sf::Text> pauseText;
    std::vector<std::unique_ptr<Button>> buttons;
    std::unique_ptr<Button> resumeButton;
    std::unique_ptr<Button> mainMenuButton;
    size_t selectedButtonIndex{0};  // Track currently selected button
};

} // namespace game
