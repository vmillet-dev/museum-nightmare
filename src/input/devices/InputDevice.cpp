#include "InputDevice.hpp"
#include "../InputManager.hpp"

namespace game {

void InputDevice::notifyActionTriggered(Action action) {
    if (auto callback = InputManager::getInstance().getActionCallback(action)) {
        callback();
    }
}

} // namespace game
