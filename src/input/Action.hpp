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
    Fire
};

class ActionUtil {
public:
    static std::string toString(Action action) {
        try {
            return actionMap.get_left(action);
        } catch (const std::out_of_range&) {
            throw std::invalid_argument("Unknown Action value");
        }
    }

    static Action fromString(const std::string& actionStr) {
        try {
            return actionMap.get_right(actionStr);
        } catch (const std::out_of_range&) {
            throw std::invalid_argument("Unknown action string: " + actionStr);
        }
    }

    static const Bimap<Action, std::string>& getActionMap() {
        return actionMap;
    }

private:
    inline static const Bimap<Action, std::string> actionMap = []() {
        Bimap<Action, std::string> m;
        m.insert(Action::MoveUp, "MoveUp");
        m.insert(Action::MoveDown, "MoveDown");
        m.insert(Action::MoveLeft, "MoveLeft");
        m.insert(Action::MoveRight, "MoveRight");
        m.insert(Action::Pause, "Pause");
        m.insert(Action::Confirm, "Confirm");
        m.insert(Action::Cancel, "Cancel");
        m.insert(Action::Fire, "Fire");
        return m;
    }();
};

} // namespace game
