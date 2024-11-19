#pragma once
#include "../objects/GameObjectManager.hpp"
#include "../objects/Wall.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <spdlog/spdlog.h>

namespace game {

struct LevelObject {
    std::string type;
    float x, y;
    float width, height;
};

class LevelLoader {
public:
    static void loadLevelFromFile(const std::string& filename, GameObjectManager& gameObjects) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open level file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments

            std::istringstream iss(line);
            LevelObject obj;
            if (!(iss >> obj.type >> obj.x >> obj.y >> obj.width >> obj.height)) {
                spdlog::warn("Invalid line in level file: {}", line);
                continue;
            }

            if (obj.type == "Wall") {
                gameObjects.addObject(std::make_unique<Wall>(obj.x, obj.y, obj.width, obj.height));
            }
            // Add more object types here as needed
        }

        spdlog::info("Level loaded successfully from file: {}", filename);
    }
};

} // namespace game
