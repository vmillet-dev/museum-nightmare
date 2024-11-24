#pragma once
#include "Screen.hpp"
#include "PauseScreen.hpp"
#include "../game/GameObjectManager.hpp"
#include "../game/objects/actor/Player.hpp"
#include "../game/objects/Wall.hpp"
#include "../input/InputManager.hpp"
#include "../core/Game.hpp"
#include "../game/levels/LevelLoader.hpp"

namespace game {

class Game;  // Forward declaration

class GameScreen : public Screen {
public:
    GameScreen(Game& game, const std::string& levelFile = "assets/levels/level1.toml");
    GameScreen(Game& game, bool procedural);
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void pause() override;
    void resume() override;

private:
    void loadLevel(const std::string& levelFile);
    void loadProceduralLevel();
    Game& game;
    std::unique_ptr<GameObjectManager> gameObjectManager;
    Player* playerPtr{nullptr};  // Non-owning pointer to player
};

} // namespace game
