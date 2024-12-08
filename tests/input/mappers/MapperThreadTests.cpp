#include <catch2/catch_test_macros.hpp>
#include <thread>
#include <vector>
#include <atomic>
#include "../../../src/input/mappers/KeyMapper.hpp"
#include "../../../src/input/mappers/MouseMapper.hpp"
#include "../../../src/input/mappers/ControllerMapper.hpp"

using namespace game;

TEST_CASE("KeyMapper thread-safe initialization", "[thread][KeyMapper]") {
    std::vector<std::thread> threads;
    std::atomic<int> successful_lookups{0};

    // Create multiple threads that will try to initialize and use the mapper
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&successful_lookups]() {
            try {
                auto key = KeyMapper::fromName("A");
                auto name = KeyMapper::toName(key);
                if (name == "A") {
                    ++successful_lookups;
                }
            } catch (const std::exception&) {
                // Initialization failure would throw
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    REQUIRE(successful_lookups > 0);
}

TEST_CASE("MouseMapper thread-safe initialization", "[thread][MouseMapper]") {
    std::vector<std::thread> threads;
    std::atomic<int> successful_lookups{0};

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&successful_lookups]() {
            try {
                auto button = MouseMapper::stringToButton("LeftButton");
                auto name = MouseMapper::buttonToString(button);
                if (name == "LeftButton") {
                    ++successful_lookups;
                }
            } catch (const std::exception&) {
                // Initialization failure would throw
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    REQUIRE(successful_lookups > 0);
}

TEST_CASE("ControllerMapper thread-safe initialization", "[thread][ControllerMapper]") {
    std::vector<std::thread> threads;
    std::atomic<int> successful_lookups{0};

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&successful_lookups]() {
            try {
                auto button = ControllerMapper::mapButtonName("A");
                auto name = ControllerMapper::mapButtonId(button);
                if (name == "A") {
                    ++successful_lookups;
                }
            } catch (const std::exception&) {
                // Initialization failure would throw
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    REQUIRE(successful_lookups > 0);
}

TEST_CASE("Bimap concurrent access safety", "[thread][Bimap]") {
    game::Bimap<int, std::string> bimap;
    std::vector<std::thread> threads;
    std::atomic<int> successful_inserts{0};
    std::atomic<int> successful_lookups{0};

    // Pre-populate some data
    bimap.insert(1, "one");
    bimap.insert(2, "two");

    // Create threads that will perform concurrent operations
    for (int i = 0; i < 5; ++i) {
        // Writer threads
        threads.emplace_back([&bimap, i, &successful_inserts]() {
            if (bimap.insert(i + 10, "value" + std::to_string(i))) {
                ++successful_inserts;
            }
        });

        // Reader threads
        threads.emplace_back([&bimap, &successful_lookups]() {
            try {
                auto value = bimap.get_left(1);
                if (value == "one") {
                    ++successful_lookups;
                }
            } catch (const std::exception&) {}
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    REQUIRE(successful_inserts > 0);
    REQUIRE(successful_lookups > 0);
    REQUIRE(bimap.size() == 2 + successful_inserts);
}
