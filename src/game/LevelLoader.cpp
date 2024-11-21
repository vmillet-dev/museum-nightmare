#include "LevelLoader.hpp"
#include "objects/Wall.hpp"
#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

namespace game {

bool LevelLoader::loadLevel(const std::string& filename, GameObjectManager& manager) {
    try {
        auto grid = parseLevel(filename);
        createObjects(grid, manager);
        return true;
    } catch (const std::exception& e) {
        spdlog::error("Failed to load level {}: {}", filename, e.what());
        return false;
    }
}

std::vector<std::vector<int>> LevelLoader::parseLevel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open level file: " + filename);
    }

    std::vector<std::vector<int>> grid;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::istringstream iss(line);
        int tile;
        while (iss >> tile) {
            row.push_back(tile);
        }
        if (!row.empty()) {
            grid.push_back(row);
        }
    }
    return grid;
}

void LevelLoader::createObjects(const std::vector<std::vector<int>>& grid, GameObjectManager& manager) {
    const float TILE_SIZE = 64.0f;

    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[y].size(); ++x) {
            float posX = x * TILE_SIZE;
            float posY = y * TILE_SIZE;

            if (grid[y][x] == 1) {
                auto wall = std::make_unique<Wall>(posX, posY, TILE_SIZE, TILE_SIZE);
                manager.addObject(std::move(wall));
                spdlog::debug("Created wall at ({}, {})", posX, posY);
            }
            // Note: Other object types will be handled by their respective systems
            // (e.g., Player creation is handled by the GameScreen)
        }
    }
}

} // namespace game
