#pragma once
#include <string>
#include "../core/containers/bimap.hpp"
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
        try {
            return actionMap().get_left(action);
        } catch (const std::out_of_range&) {
            throw std::invalid_argument("Unknown Action value");
        }
    }

    // Convert string to Action enum
    static Action fromString(const std::string& actionStr) {
        try {
            return actionMap().get_right(actionStr);
        } catch (const std::out_of_range&) {
            throw std::invalid_argument("Unknown action string: " + actionStr);
        }
    }

    // Get all available actions and their string representations
    static const Bimap<Action, std::string>& getActionMap() {
        return actionMap();
    }

private:
    // Lazy initialization for bidirectional action mapping
    static const Bimap<Action, std::string>& actionMap() {
        static const Bimap<Action, std::string> map = []() {
            Bimap<Action, std::string> m;
            m.insert(Action::MoveUp, "MoveUp");
            m.insert(Action::MoveDown, "MoveDown");
            m.insert(Action::MoveLeft, "MoveLeft");
            m.insert(Action::MoveRight, "MoveRight");
            m.insert(Action::Pause, "Pause");
            m.insert(Action::Confirm, "Confirm");
            m.insert(Action::Cancel, "Cancel");
            m.insert(Action::Fire, "Fire");
            m.insert(Action::MouseLeft, "MouseLeft");
            m.insert(Action::MouseRight, "MouseRight");
            return m;
        }();
        return map;
    }
};

} // namespace game
