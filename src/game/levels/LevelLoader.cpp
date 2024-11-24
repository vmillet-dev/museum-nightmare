#include "LevelLoader.hpp"
#include "../../core/Game.hpp"
#include "../objects/Wall.hpp"
#include "../objects/actor/Player.hpp"
#include "../objects/actor/Enemy.hpp"
#include <spdlog/spdlog.h>
#include <fstream>

namespace game {

LevelData LevelLoader::loadFromFile(const std::string& filename) {
    LevelData data;
    try {
        auto config = toml::parse_file(filename);
        const auto& level = config["level"];

        data.cellSize = level["cell_size"].value_or(64.0f);
        const auto& gridArray = level["grid"].as_array();

        // Convert TOML array to our grid format
        for (const auto& row : *gridArray) {
            std::vector<int> gridRow;
            for (const auto& cell : *row.as_array()) {
                gridRow.push_back(cell.as_integer()->get());
            }
            data.grid.push_back(gridRow);
        }

        validateLevel(data);
    } catch (const std::exception& e) {
        spdlog::error("Failed to load level file {}: {}", filename, e.what());
        throw;
    }
    return data;
}

void LevelLoader::createGameObjects(const LevelData& data, GameObjectManager& manager, Game& game) {
    for (size_t y = 0; y < data.grid.size(); ++y) {
        for (size_t x = 0; x < data.grid[y].size(); ++x) {
            int type = data.grid[y][x];
            if (type != 0) { // Skip empty cells
                float worldX = x * data.cellSize + data.cellSize / 2;
                float worldY = y * data.cellSize + data.cellSize / 2;
                auto object = createGameObject(type, worldX, worldY, game);
                if (object) {
                    manager.addObject(std::move(object));
                }
            }
        }
    }
}

std::unique_ptr<GameObject> LevelLoader::createGameObject(int type, float x, float y, Game& game) {
    switch (type) {
        case 1: // Wall
            return std::make_unique<Wall>(x, y, 64.0f, 64.0f);
        case 2: // Player
            return std::make_unique<Player>(x, y, game);
        case 3: // Enemy
            return std::make_unique<Enemy>(x, y);
        default:
            spdlog::warn("Unknown object type: {}", type);
            return nullptr;
    }
}

void LevelLoader::validateLevel(const LevelData& data) {
    if (data.grid.empty() || data.grid[0].empty()) {
        throw std::runtime_error("Level grid cannot be empty");
    }

    // Check that all rows have the same width
    size_t width = data.grid[0].size();
    for (const auto& row : data.grid) {
        if (row.size() != width) {
            throw std::runtime_error("All rows in the grid must have the same width");
        }
    }
}

LevelData LevelLoader::generateProceduralLevel(float cellSize) {
    LevelData data;
    data.cellSize = cellSize;

    // Create a simple procedural level (10x8)
    const int width = 10;
    const int height = 8;

    // Initialize with empty space
    data.grid = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));

    // Add border walls
    for (int x = 0; x < width; ++x) {
        data.grid[0][x] = 1;  // Top wall
        data.grid[height-1][x] = 1;  // Bottom wall
    }
    for (int y = 0; y < height; ++y) {
        data.grid[y][0] = 1;  // Left wall
        data.grid[y][width-1] = 1;  // Right wall
    }

    // Add some random walls
    for (int i = 0; i < 5; ++i) {
        int x = 1 + (std::rand() % (width - 2));
        int y = 1 + (std::rand() % (height - 2));
        if (data.grid[y][x] == 0) {  // Only place if empty
            data.grid[y][x] = 1;
        }
    }

    // Add player (always in top-left area)
    data.grid[1][1] = 2;

    // Add enemy (always in bottom-right area)
    data.grid[height-2][width-2] = 3;

    return data;
}

} // namespace game
