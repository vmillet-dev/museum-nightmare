#pragma once
#include "Screen.hpp"
#include "../ui/Button.hpp"
#include "ScreenManager.hpp"
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>

namespace game {

class Game;  // Forward declaration

class PauseScreen : public Screen {
public:
    PauseScreen(Game& game);

    void handleInput(const sf::Event& event) override {
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
            for (auto& button : buttons) {
                button->handleInput(mousePos);
            }
        }
    }

    void update(float deltaTime) override {}

    void render(sf::RenderWindow& window) override {
        // Semi-transparent background
        sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
        overlay.setFillColor(sf::Color(0, 0, 0, 128));
        window.draw(overlay);

        for (auto& button : buttons) {
            button->render(window);
        }
    }

private:
    std::vector<std::unique_ptr<Button>> buttons;
    Game& game;
};

} // namespace game
