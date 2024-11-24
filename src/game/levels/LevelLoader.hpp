#pragma once
#include "LevelData.hpp"
#include "../GameObjectManager.hpp"
#include <string>
#include <memory>
#include <toml++/toml.h>

namespace game {

class Game;  // Forward declaration

class LevelLoader {
public:
    static LevelData loadFromFile(const std::string& filename);
    static void createGameObjects(const LevelData& data, GameObjectManager& manager, Game& game);
    static LevelData generateProceduralLevel(float cellSize);

private:
    static std::unique_ptr<GameObject> createGameObject(int type, float x, float y, Game& game);
    static void validateLevel(const LevelData& data);
};

} // namespace game
