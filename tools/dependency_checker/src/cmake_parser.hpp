#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <optional>

struct Dependency {
    std::string name;
    std::string repo;
    std::string tag;
    std::string variable;
    std::string latest_version;
};

class CMakeParser {
public:
    static std::pair<std::vector<Dependency>, std::string> parseDependencies(const std::filesystem::path& file_path);
    static void updateFile(const std::string& content, const std::vector<Dependency>& dependencies,
                          const std::filesystem::path& file_path);
private:
    static std::unordered_map<std::string, std::string> parseVersionVariables(const std::string& content);
};
