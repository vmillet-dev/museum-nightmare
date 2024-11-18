#include "core/Game.hpp"
#include <spdlog/spdlog.h>

int main() {
    try {
        spdlog::set_level(spdlog::level::debug);
        spdlog::info("Starting game application");

        game::Game game;
        game.run();

        spdlog::info("Game closed successfully");
        return 0;
    } catch (const std::exception& e) {
        spdlog::error("Fatal error: {}", e.what());
        return 1;
    }
}
