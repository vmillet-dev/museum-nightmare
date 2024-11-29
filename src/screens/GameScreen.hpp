#pragma once
#include "Screen.hpp"
#include "PauseScreen.hpp"
#include "../game/GameObjectManager.hpp"
#include "../game/objects/actor/Player.hpp"
#include "../game/objects/Wall.hpp"
#include "../game/Camera.hpp"
#include "../game/LevelLoader.hpp"
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
    std::unique_ptr<GameObjectManager> gameObjectManager;
    Player* playerPtr{nullptr};  // Non-owning pointer to player
    std::unique_ptr<Camera> camera;
};

} // namespace game
