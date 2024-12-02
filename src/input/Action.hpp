#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>

namespace game {

enum class Action {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Pause,
    Confirm,
    Cancel,
    Fire,
    MouseLeft,
    MouseRight
};

// Convert Action enum to string
inline std::string toString(Action action) {
    switch (action) {
        case Action::MoveUp:     return "MoveUp";
        case Action::MoveDown:   return "MoveDown";
        case Action::MoveLeft:   return "MoveLeft";
        case Action::MoveRight:  return "MoveRight";
        case Action::Pause:      return "Pause";
        case Action::Confirm:    return "Confirm";
        case Action::Cancel:     return "Cancel";
        case Action::Fire:       return "Fire";
        case Action::MouseLeft:  return "MouseLeft";
        case Action::MouseRight: return "MouseRight";
        default: throw std::invalid_argument("Unknown Action value");
    }
}

// Convert string to Action enum
inline Action fromString(const std::string& actionStr) {
    static const std::unordered_map<std::string, Action> actionMap = {
        {"MoveUp", Action::MoveUp},
        {"MoveDown", Action::MoveDown},
        {"MoveLeft", Action::MoveLeft},
        {"MoveRight", Action::MoveRight},
        {"Pause", Action::Pause},
        {"Confirm", Action::Confirm},
        {"Cancel", Action::Cancel},
        {"Fire", Action::Fire},
        {"MouseLeft", Action::MouseLeft},
        {"MouseRight", Action::MouseRight}
    };

    auto it = actionMap.find(actionStr);
    if (it != actionMap.end()) {
        return it->second;
    }
    throw std::invalid_argument("Unknown action string: " + actionStr);
}

// Get all available actions and their string representations
inline const std::unordered_map<std::string, Action>& getActionMap() {
    static const std::unordered_map<std::string, Action> actionMap = {
        {"MoveUp", Action::MoveUp},
        {"MoveDown", Action::MoveDown},
        {"MoveLeft", Action::MoveLeft},
        {"MoveRight", Action::MoveRight},
        {"Pause", Action::Pause},
        {"Confirm", Action::Confirm},
        {"Cancel", Action::Cancel},
        {"Fire", Action::Fire},
        {"MouseLeft", Action::MouseLeft},
        {"MouseRight", Action::MouseRight}
    };
    return actionMap;
}

} // namespace game
