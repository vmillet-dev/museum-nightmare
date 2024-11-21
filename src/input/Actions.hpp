#pragma once

namespace game {

enum class Action {
    None,       // Default/no action state
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Pause,
    Confirm,
    Cancel
};

} // namespace game
