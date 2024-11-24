#pragma once
#include <SFML/System/Vector2.hpp>

namespace game {
namespace GridUtils {

inline sf::Vector2f gridToWorld(int x, int y, float cellSize) {
    return sf::Vector2f(x * cellSize + cellSize/2, y * cellSize + cellSize/2);
}

inline sf::Vector2i worldToGrid(float x, float y, float cellSize) {
    return sf::Vector2i(static_cast<int>(x / cellSize),
                       static_cast<int>(y / cellSize));
}

} // namespace GridUtils
} // namespace game
