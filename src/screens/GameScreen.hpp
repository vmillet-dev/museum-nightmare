#pragma once
#include "Screen.hpp"
#include "PauseScreen.hpp"
#include "../game/GameObjectManager.hpp"
#include "../game/objects/actor/Player.hpp"
#include "../game/objects/Wall.hpp"
#include "../game/level/LevelLoader.hpp"
#include "../game/Camera.hpp"
#include "../input/InputManager.hpp"
#include "../core/Game.hpp"

namespace game {

class Game;  // Forward declaration

class GameScreen : public Screen {
public:
    GameScreen(Game& game);
    ~GameScreen();  // Add destructor declaration
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void pause() override;
    void resume() override;

private:
    Game& game;
    std::unique_ptr<GameObjectManager> gameObjectManager;
    std::unique_ptr<Camera> camera;
    std::vector<std::unique_ptr<GameObject>> parallaxLayers;
    Player* playerPtr{nullptr};  // Non-owning pointer to player
    b2WorldId worldId{b2_nullWorldId};  // Add Box2D world ID

    void loadLevel(const std::string& mapFile);
};

} // namespace game
