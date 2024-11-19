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

        // Handle real events first
        handleEvents();

        // Test sequence logic
        testInputTimer += dt;
        if (testInputTimer >= 2.0f && !testSequenceActive) {
            spdlog::info("Starting input test sequence...");
            testSequenceActive = true;
            currentTestStep = 0;
            stepTimer = 0.0f;
            testInputTimer = 0.0f;
            keyPressed = false;
        }

        if (testSequenceActive) {
            stepTimer += dt;

            // Keep the current test key "pressed" for most of the step duration
            if (currentTestStep < testSequence.size()) {
                if (stepTimer < TEST_STEP_DURATION * 0.8f && !keyPressed) {
                    // Simulate key press at start of step
                    sf::Event keyEvent;
                    keyEvent.type = sf::Event::KeyPressed;
                    keyEvent.key.code = testSequence[currentTestStep];
                    keyEvent.key.alt = false;
                    keyEvent.key.control = false;
                    keyEvent.key.shift = false;
                    keyEvent.key.system = false;

                    switch (testSequence[currentTestStep]) {
                        case sf::Keyboard::Z:
                            spdlog::debug("Test sequence: Move Up (Z)");
                            break;
                        case sf::Keyboard::S:
                            spdlog::debug("Test sequence: Move Down (S)");
                            break;
                        case sf::Keyboard::Q:
                            spdlog::debug("Test sequence: Move Left (Q)");
                            break;
                        case sf::Keyboard::D:
                            spdlog::debug("Test sequence: Move Right (D)");
                            break;
                    }

                    // Process the simulated key event and set key state
                    handleEvent(keyEvent);
                    InputManager::getInstance().setKeyState(testSequence[currentTestStep], true);
                    keyPressed = true;
                } else if (stepTimer >= TEST_STEP_DURATION) {
                    // Release key and move to next step
                    InputManager::getInstance().setKeyState(testSequence[currentTestStep], false);
                    currentTestStep++;
                    stepTimer = 0.0f;
                    keyPressed = false;

                    if (currentTestStep >= testSequence.size()) {
                        testSequenceActive = false;
                        spdlog::info("Input test sequence completed");
                    }
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
            case sf::Keyboard::Escape: keyName = "Escape"; break;
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
