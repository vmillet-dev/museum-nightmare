#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../input/InputManager.hpp"
#include "../game/levels/Level2.hpp"
#include "../game/levels/LevelLoader.hpp"
#include <spdlog/spdlog.h>

namespace game {

GameScreen::GameScreen(Game& game, const std::string& levelFile) : game(game) {
    spdlog::info("Initializing game screen");
    gameObjectManager = std::make_unique<GameObjectManager>();
    loadLevel(levelFile);
}

GameScreen::GameScreen(Game& game, bool procedural) : game(game) {
    spdlog::info("Initializing game screen with procedural level");
    gameObjectManager = std::make_unique<GameObjectManager>();
    if (procedural) {
        loadProceduralLevel();
    } else {
        loadLevel("assets/levels/level1.toml");
    }
}

void GameScreen::loadLevel(const std::string& levelFile) {
    auto levelData = LevelLoader::loadFromFile(levelFile);
    LevelLoader::createGameObjects(levelData, *gameObjectManager, game);

    // Find and store player pointer
    for (const auto& obj : gameObjectManager->getObjects()) {
        if (auto player = dynamic_cast<Player*>(obj.get())) {
            playerPtr = player;
            break;
        }
    }
}

void GameScreen::loadProceduralLevel() {
    auto levelData = LevelLoader::generateProceduralLevel(64.0f);
    LevelLoader::createGameObjects(levelData, *gameObjectManager, game);

    // Find and store player pointer
    for (const auto& obj : gameObjectManager->getObjects()) {
        if (auto player = dynamic_cast<Player*>(obj.get())) {
            playerPtr = player;
            break;
        }
    }
}

void GameScreen::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        spdlog::info("Opening pause menu");
        ScreenManager::getInstance().pushScreen(std::make_unique<PauseScreen>(game));
    }
}

void GameScreen::update(float deltaTime) {
    // Update all game objects
    gameObjectManager->update(deltaTime);
}

void GameScreen::render(sf::RenderWindow& window) {
    gameObjectManager->render(window);
}

void GameScreen::pause() { spdlog::info("Game state: paused"); }

void GameScreen::resume() { spdlog::info("Game state: resumed"); }

} // namespace game
