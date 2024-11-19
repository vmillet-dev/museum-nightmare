#include "Game.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include <spdlog/spdlog.h>
#include <vector>

namespace game {

Game::Game() : window(
    sf::VideoMode(
        ConfigManager::getInstance().getWindowWidth(),
        ConfigManager::getInstance().getWindowHeight()
    ),
    ConfigManager::getInstance().getWindowTitle()
) {
    spdlog::info("Initializing game...");
    auto& configManager = ConfigManager::getInstance();
    auto& inputManager = InputManager::getInstance();
    inputManager.init();
    ScreenManager::getInstance().pushScreen(std::make_unique<GameScreen>(*this));
    spdlog::info("Game initialized successfully");
}

void Game::run() {
    sf::Clock clock;
    sf::Clock fpsTimer;
    float testInputTimer = 0.0f;
    int frameCount = 0;
    bool testSequenceActive = false;
    int currentTestStep = 0;
    const float TEST_STEP_DURATION = 0.5f;
    float stepTimer = 0.0f;
    bool keyPressed = false;

    // Initialize input system
    InputManager::getInstance().init();

    // Define test sequence for input system verification
    const std::vector<std::pair<std::string, Action>> testSequence = {
        {"Move Up", Action::MoveUp},
        {"Move Down", Action::MoveDown},
        {"Move Left", Action::MoveLeft},
        {"Move Right", Action::MoveRight}
    };

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // Handle real events first
        handleEvents();

        // Test sequence logic
        testInputTimer += dt;
        if (testInputTimer >= 2.0f && !testSequenceActive) {
            spdlog::info("Starting input system test sequence...");
            testSequenceActive = true;
            currentTestStep = 0;
            stepTimer = 0.0f;
            testInputTimer = 0.0f;
            keyPressed = false;
        }

        if (testSequenceActive) {
            stepTimer += dt;

            // Test each action in sequence
            if (currentTestStep < testSequence.size()) {
                auto& [actionName, action] = testSequence[currentTestStep];

                if (stepTimer < TEST_STEP_DURATION * 0.8f && !keyPressed) {
                    spdlog::debug("Testing action: {}", actionName);
                    keyPressed = true;
                } else if (stepTimer >= TEST_STEP_DURATION) {
                    // Move to next test
                    currentTestStep++;
                    stepTimer = 0.0f;
                    keyPressed = false;

                    if (currentTestStep >= testSequence.size()) {
                        testSequenceActive = false;
                        spdlog::info("Input system test sequence completed");
                    }
                }

                // Check if action is registered
                if (InputManager::getInstance().isActionPressed(action)) {
                    spdlog::debug("Action {} detected", actionName);
                }
            }
        }

        update(deltaTime.asSeconds());
        render();

        // FPS Counter
        frameCount++;
        if (fpsTimer.getElapsedTime().asSeconds() >= 1.0f) {
            spdlog::debug("FPS: {}", frameCount);
            frameCount = 0;
            fpsTimer.restart();
        }
    }
}

void Game::handleEvent(const sf::Event& event) {
    // Handle window close
    if (event.type == sf::Event::Closed) {
        spdlog::info("Window close requested");
        window.close();
        return;
    }

    // Handle key events
    if (event.type == sf::Event::KeyPressed) {
        std::string keyName;
        switch (event.key.code) {
            case sf::Keyboard::Z: keyName = "Move Up (Z)"; break;
            case sf::Keyboard::S: keyName = "Move Down (S)"; break;
            case sf::Keyboard::Q: keyName = "Move Left (Q)"; break;
            case sf::Keyboard::D: keyName = "Move Right (D)"; break;
            case sf::Keyboard::Escape:
                keyName = "Escape";
                spdlog::info("Escape pressed, quitting game");
                window.close();
                break;
            default: return; // Skip logging unknown keys
        }
        spdlog::debug("Key pressed: {}", keyName);
    }

    // Let current screen handle the event
    if (!ScreenManager::getInstance().isEmpty()) {
        ScreenManager::getInstance().handleInput(event);
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        handleEvent(event);
    }
}

void Game::update(float deltaTime) {
    InputManager::getInstance().update();
    if (!ScreenManager::getInstance().isEmpty()) {
        ScreenManager::getInstance().update(deltaTime);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    if (!ScreenManager::getInstance().isEmpty()) {
        ScreenManager::getInstance().render(window);
    }
    window.display();
}

void Game::quit() {
    window.close();
}

sf::RenderWindow& Game::getWindow() {
    return window;
}

} // namespace game
