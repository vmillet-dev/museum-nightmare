#include "core/Game.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>

void setup_logging() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("game.log", true);

    // Set log levels
    console_sink->set_level(spdlog::level::debug);
    file_sink->set_level(spdlog::level::debug);

    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("game_logger", sinks.begin(), sinks.end());

    logger->flush_on(spdlog::level::debug);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);
}


int main(int argc, char* argv[]) {
    setup_logging();

    try {
        bool test_mode = (argc > 1 && std::string(argv[1]) == "--test-mode");
        float timeout = (test_mode ? 5.0f : 0.0f);  // 5 second timeout in test mode

        spdlog::info("Starting game application {} {}",
            test_mode ? "in test mode" : "",
            test_mode ? "(with 5s timeout)" : "");

        game::Game game;
        game.run(timeout);

        spdlog::info("Game closed successfully");
        return 0;
    } catch (const std::exception& e) {
        spdlog::error("Fatal error: {}", e.what());
        return 1;
    }
}
