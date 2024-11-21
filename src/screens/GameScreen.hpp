#pragma once
#include "Screen.hpp"
#include "PauseScreen.hpp"
#include "../game/objects/GameObjectManager.hpp"
#include "../game/objects/Player.hpp"
#include "../game/objects/Wall.hpp"
#include "../input/InputManager.hpp"
#include "../core/Game.hpp"

namespace game {

class Game;  // Forward declaration

class GameScreen : public Screen {
public:
    GameScreen(Game& game);
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void pause() override;
    void resume() override;

private:
    Game& game;
    GameObjectManager* gameObjectManager;  // Reference to singleton
    Player* playerPtr{nullptr};  // Non-owning pointer to player
};

} // namespace game
