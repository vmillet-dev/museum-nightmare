#pragma once
#include "../objects/GameObjectManager.hpp"
#include "../objects/Wall.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <spdlog/spdlog.h>

namespace game {

class GridLevelLoader {
public:
    static void loadLevelFromFile(const std::string& filename, GameObjectManager& gameObjects) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open level file: " + filename);
        }

        std::vector<std::string> grid;
        std::string line;
        size_t width = 0;

        // Read grid from file
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue; // Skip comments and empty lines

            // Remove any whitespace
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

            if (width == 0) {
                width = line.length();
            } else if (line.length() != width) {
                throw std::runtime_error("Inconsistent grid width in level file");
            }

            grid.push_back(line);
        }

        if (grid.empty()) {
            throw std::runtime_error("Empty level file");
        }

        const float CELL_SIZE = 32.0f; // Size of each grid cell
        const float OFFSET_X = CELL_SIZE / 2.0f;
        const float OFFSET_Y = CELL_SIZE / 2.0f;

        // Create objects based on grid
        for (size_t y = 0; y < grid.size(); ++y) {
            for (size_t x = 0; x < width; ++x) {
                float posX = x * CELL_SIZE + OFFSET_X;
                float posY = y * CELL_SIZE + OFFSET_Y;

                switch (grid[y][x]) {
                    case '1': // Wall
                        gameObjects.addObject(std::make_unique<Wall>(
                            posX, posY, CELL_SIZE, CELL_SIZE));
                        break;
                    case '0': // Floor (no object needed)
                        break;
                    default:
                        spdlog::warn("Unknown grid cell type: {}", grid[y][x]);
                        break;
                }
            }
        }

        spdlog::info("Grid level loaded successfully from file: {}", filename);
    }
};

} // namespace game
