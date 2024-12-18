#include "cmake_parser.hpp"
#include <regex>
#include <fstream>
#include <sstream>
#include <iostream>

std::string escape_regex(const std::string& str) {
    static const std::string special_chars = "[](){}.*+?|^$\\";
    std::string escaped;
    for (char c : str) {
        if (special_chars.find(c) != std::string::npos) {
            escaped += '\\';
        }
        escaped += c;
    }
    return escaped;
}

std::unordered_map<std::string, std::string> CMakeParser::parseVersionVariables(const std::string& content) {
    std::unordered_map<std::string, std::string> variables;
    std::regex set_pattern(R"###(set\s*\(\s*([A-Za-z0-9_]+_VERSION)\s*"([^"]+)"\s*\))###");

    std::sregex_iterator begin(content.begin(), content.end(), set_pattern);
    std::sregex_iterator end;

    for (auto i = begin; i != end; ++i) {
        std::smatch match = *i;
        variables[match[1].str()] = match[2].str();
        std::cout << "Found version variable: " << match[1].str() << " = " << match[2].str() << "\n";
    }

    return variables;
}

std::pair<std::vector<Dependency>, std::string> CMakeParser::parseDependencies(const std::filesystem::path& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::cout << "File contents:\n" << content << "\n=== END FILE CONTENTS ===\n";

    auto variables = parseVersionVariables(content);
    std::vector<Dependency> dependencies;

    size_t pos = 0;
    while ((pos = content.find("FetchContent_Declare", pos)) != std::string::npos) {
        size_t open_paren = content.find('(', pos);
        if (open_paren == std::string::npos) break;

        size_t close_paren = open_paren + 1;
        int paren_count = 1;
        while (paren_count > 0 && close_paren < content.length()) {
            if (content[close_paren] == '(') paren_count++;
            else if (content[close_paren] == ')') paren_count--;
            close_paren++;
        }

        if (paren_count > 0) {
            std::cerr << "Error: Unmatched parentheses in FetchContent_Declare block\n";
            break;
        }

        std::string block = content.substr(open_paren + 1, close_paren - open_paren - 2);

        std::regex name_pattern(R"(\s*(\w+)\s*)");
        std::regex repo_pattern(R"(GIT_REPOSITORY\s+([^\s\n]+))");
        std::regex tag_pattern(R"(GIT_TAG\s+(?:v?\$\{([\w_]+)\}|v?([^\s\n\)]+)))");

        std::smatch name_match, repo_match, tag_match;

        if (std::regex_search(block, name_match, name_pattern) &&
            std::regex_search(block, repo_match, repo_pattern) &&
            std::regex_search(block, tag_match, tag_pattern)) {

            Dependency dep;
            dep.name = name_match[1].str();
            dep.repo = std::regex_replace(repo_match[1].str(), std::regex(R"(^"|"$)"), "");

            if (tag_match[1].matched) {
                std::string var_name = tag_match[1].str();
                if (variables.count(var_name)) {
                    dep.variable = var_name;
                    dep.tag = variables[var_name];
                    std::cout << "  Resolved variable " << var_name << " to: " << dep.tag << "\n";
                }
            } else if (tag_match[2].matched) {
                dep.tag = tag_match[2].str();
                dep.variable = "";
            }

            if (!dep.tag.empty() && block.find("GIT_TAG v") != std::string::npos && dep.tag[0] != 'v') {
                dep.tag = "v" + dep.tag;
            }

            if (!dep.name.empty() && !dep.repo.empty() && !dep.tag.empty()) {
                std::cout << "Found dependency:\n";
                std::cout << "  Name: " << dep.name << "\n";
                std::cout << "  Repo: " << dep.repo << "\n";
                std::cout << "  Tag:  " << dep.tag << "\n";
                dependencies.push_back(dep);
            }
        }

        pos = close_paren;
    }

    if (dependencies.empty()) {
        std::cout << "No dependencies found in file.\n";
    }

    return {dependencies, content};
}

void CMakeParser::updateFile(const std::string& content, const std::vector<Dependency>& dependencies,
                           const std::filesystem::path& file_path) {
    std::string updated_content = content;
    bool updated = false;

    for (const auto& dep : dependencies) {
        if (dep.latest_version.empty()) continue;

        if (dep.variable.empty()) {
            std::regex tag_pattern("GIT_TAG\\s+" + escape_regex(dep.tag));
            updated_content = std::regex_replace(updated_content, tag_pattern,
                                              "GIT_TAG " + dep.latest_version);
        } else {
            std::string new_version = dep.latest_version;
            if (new_version[0] == 'v') {
                new_version = new_version.substr(1);
            }

            std::regex version_pattern(R"###(set\(\s*)###" + dep.variable + R"###(\s+"[^"]+"\))###");
            updated_content = std::regex_replace(updated_content, version_pattern,
                                              "set(" + dep.variable + " \"" + new_version + "\")");
        }
        updated = true;
    }

    if (updated) {
        std::ofstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + file_path.string());
        }
        file << updated_content;
    }
}
