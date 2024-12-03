#pragma once
#include "Screen.hpp"
#include "../ui/Button.hpp"
#include "../ui/MenuBuilder.hpp"
#include "ScreenManager.hpp"
#include <memory>
#include <spdlog/spdlog.h>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

namespace game {

class Game;  // Forward declaration
class MainMenuScreen;  // Forward declaration

class PauseScreen : public Screen {
public:
    explicit PauseScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    MenuBuilder menuBuilder_;
    Button* resumeButton;
    Button* mainMenuButton;
    sf::Font font;
    sf::Text pauseText;
};

} // namespace game
