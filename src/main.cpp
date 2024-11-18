#include "core/Game.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main() {
    try {
        // Set up multi-sink logging (console and file)
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("game.log", true);

        // Set flush policy
        console_sink->set_level(spdlog::level::debug);
        file_sink->set_level(spdlog::level::debug);

        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
        auto logger = std::make_shared<spdlog::logger>("game_logger", sinks.begin(), sinks.end());
        logger->flush_on(spdlog::level::debug);  // Flush on all messages
        spdlog::set_default_logger(logger);
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
