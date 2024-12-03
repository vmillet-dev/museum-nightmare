#pragma once
#include "../ui/MenuBuilder.hpp"
#include "Screen.hpp"
#include <vector>

namespace game {

class SettingsScreen : public Screen {
public:
    explicit SettingsScreen(Game& game);
    void update(InputManager& inputManager) override;
    void render(sf::RenderWindow& window) override;

private:
    void initializeMenu();
    void handleResolutionChange(size_t index);

    MenuBuilder menuBuilder;
    std::vector<std::unique_ptr<Widget>> widgets;
    std::vector<sf::Vector2u> availableResolutions;
};

} // namespace game
