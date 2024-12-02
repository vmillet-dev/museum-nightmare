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

class ActionUtil {
public:
    // Convert Action enum to string
    static std::string toString(Action action) {
        auto it = actionToStringMap().find(action);
        if (it != actionToStringMap().end()) {
            return it->second;
        }
        throw std::invalid_argument("Unknown Action value");
    }

    // Convert string to Action enum
    static Action fromString(const std::string& actionStr) {
        auto it = stringToActionMap().find(actionStr);
        if (it != stringToActionMap().end()) {
            return it->second;
        }
        throw std::invalid_argument("Unknown action string: " + actionStr);
    }

    // Get all available actions and their string representations
    static const std::unordered_map<std::string, Action>& getActionMap() {
        return stringToActionMap();
    }

private:
    // Lazy initialization for string to Action map
    static const std::unordered_map<std::string, Action>& stringToActionMap() {
        static const std::unordered_map<std::string, Action> map = {
            {"MoveUp", Action::MoveUp},
            {"MoveDown", Action::MoveDown},
            {"MoveLeft", Action::MoveLeft},
            {"MoveRight", Action::MoveRight},
            {"Pause", Action::Pause},
            {"Confirm", Action::Confirm},
            {"Cancel", Action::Cancel},
            {"Fire", Action::Fire}
            //{"MouseLeft", Action::MouseLeft},
            //{"MouseRight", Action::MouseRight}
        };
        return map;
    }

    // Lazy initialization for Action to string map
    static const std::unordered_map<Action, std::string>& actionToStringMap() {
        static const std::unordered_map<Action, std::string> map = {
            {Action::MoveUp, "MoveUp"},
            {Action::MoveDown, "MoveDown"},
            {Action::MoveLeft, "MoveLeft"},
            {Action::MoveRight, "MoveRight"},
            {Action::Pause, "Pause"},
            {Action::Confirm, "Confirm"},
            {Action::Cancel, "Cancel"},
            {Action::Fire, "Fire"}
            //{Action::MouseLeft, "MouseLeft"},
            //{Action::MouseRight, "MouseRight"}
        };
        return map;
    }
};

} // namespace game
