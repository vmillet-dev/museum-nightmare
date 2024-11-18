#include "Game.hpp"
#include "../screens/MainMenuScreen.hpp"
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
    ScreenManager::getInstance().pushScreen(std::make_unique<MainMenuScreen>(*this));
    spdlog::info("Game initialized successfully");
}

void Game::run() {
    sf::Clock clock;
    sf::Clock fpsTimer;
    float testInputTimer = 0.0f;
    int frameCount = 0;
    bool testSequenceActive = false;
    int currentTestStep = 0;
    const float TEST_STEP_DURATION = 0.5f;  // Duration for each test step
    float stepTimer = 0.0f;

    // Define test sequence keys
    const std::vector<sf::Keyboard::Key> testSequence = {
        sf::Keyboard::Z,    // Move up
        sf::Keyboard::S,    // Move down
        sf::Keyboard::Q,    // Move left
        sf::Keyboard::D     // Move right
    };

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // Test sequence logic
        testInputTimer += dt;
        if (testInputTimer >= 2.0f && !testSequenceActive) {
            spdlog::info("Starting input test sequence...");
            testSequenceActive = true;
            currentTestStep = 0;
            stepTimer = 0.0f;
            testInputTimer = 0.0f;
        }

        if (testSequenceActive) {
            stepTimer += dt;
            if (stepTimer >= TEST_STEP_DURATION) {
                currentTestStep++;
                stepTimer = 0.0f;

                if (currentTestStep >= testSequence.size()) {
                    testSequenceActive = false;
                    spdlog::info("Input test sequence completed");
                }
            } else if (currentTestStep < testSequence.size()) {
                sf::Event testEvent;
                testEvent.type = sf::Event::KeyPressed;
                testEvent.key.code = testSequence[currentTestStep];
                handleEvent(testEvent);
            }
        }

        handleEvents();
        update(deltaTime.asSeconds());
        render();

        // FPS logging
        frameCount++;
        if (fpsTimer.getElapsedTime().asSeconds() >= 1.0f) {
            spdlog::debug("FPS: {}", frameCount);
            frameCount = 0;
            fpsTimer.restart();
        }
    }
}

void Game::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        std::string keyName;
        switch (event.key.code) {
            case sf::Keyboard::Z: keyName = "Z (Up)"; break;
            case sf::Keyboard::S: keyName = "S (Down)"; break;
            case sf::Keyboard::Q: keyName = "Q (Left)"; break;
            case sf::Keyboard::D: keyName = "D (Right)"; break;
            case sf::Keyboard::Escape: keyName = "Escape"; break;
            default: keyName = "Unknown"; break;
        }
        spdlog::debug("Key pressed: {}", keyName);
    }
    if (event.type == sf::Event::Closed) {
        spdlog::info("Window close requested");
        window.close();
    }

    // Log key events
    if (event.type == sf::Event::KeyPressed) {
        std::string keyName;
        switch (event.key.code) {
            case sf::Keyboard::W: keyName = "W"; break;
            case sf::Keyboard::A: keyName = "A"; break;
            case sf::Keyboard::S: keyName = "S"; break;
            case sf::Keyboard::D: keyName = "D"; break;
            case sf::Keyboard::Escape: keyName = "Escape"; break;
            default: keyName = "Unknown"; break;
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
