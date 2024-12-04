#pragma once
#include "Screen.hpp"
#include "../ui/MenuBuilder.hpp"
#include <vector>

namespace game {

class MainMenuScreen : public Screen {
public:
    explicit MainMenuScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    MenuBuilder menuBuilder_;
    std::vector<Button*> buttons_;
    size_t selectedButtonIndex_;
};

} // namespace game
