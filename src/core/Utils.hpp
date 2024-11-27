#include <string>
#include <stdexcept>
#include "../input/Action.hpp"

namespace game {

class Utils {
public:
    static std::string actionToString(Action action) {
        switch (action) {
        case Action::MoveUp:    return "MoveUp";
        case Action::MoveDown:  return "MoveDown";
        case Action::MoveLeft:  return "MoveLeft";
        case Action::MoveRight: return "MoveRight";
        case Action::Pause:     return "Pause";
        case Action::Confirm:   return "Confirm";
        case Action::Cancel:    return "Cancel";
        case Action::MouseLeft: return "MouseLeft";
        case Action::MouseRight:return "MouseRight";
        default: throw std::invalid_argument("Unknown Action value");
        }
    }
};

}