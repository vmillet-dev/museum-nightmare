#pragma once
#include <vector>

namespace game {

struct LevelData {
    std::vector<std::vector<int>> grid;  // 0=empty, 1=wall, 2=player, 3=enemy
    float cellSize;  // Size of each grid cell in pixels

    // Helper methods to get dimensions from grid
    int getWidth() const { return grid[0].size(); }
    int getHeight() const { return grid.size(); }
};

} // namespace game
