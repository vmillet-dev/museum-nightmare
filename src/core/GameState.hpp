#pragma once

namespace game {

enum class GameState {
    MainMenu,
    Playing,
    Paused,
    Settings,  // Add Settings state
    Test,      // Add Test state for widget testing
    Quit
};

} // namespace game
