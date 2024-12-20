#include "github_api.hpp"
#include "cmake_parser.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

struct Options {
    std::string dependencies_file;
    bool dry_run = false;
};

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " --dependencies-file <path/to/dependencies.cmake> [--dry-run]\n"
              << "Options:\n"
              << "  --dependencies-file  Path to the dependencies.cmake file\n"
              << "  --dry-run           Check for updates without modifying the file\n";
}

Options parseOptions(int argc, char* argv[]) {
    Options options;
    std::vector<std::string> args(argv + 1, argv + argc);

    auto it = std::find(args.begin(), args.end(), "--dependencies-file");
    if (it != args.end() && std::next(it) != args.end()) {
        options.dependencies_file = *std::next(it);
    }

    options.dry_run = std::find(args.begin(), args.end(), "--dry-run") != args.end();

    if (options.dependencies_file.empty()) {
        throw std::runtime_error("Missing required argument: --dependencies-file");
    }

    return options;
}

int main(int argc, char* argv[]) {
    try {
        auto options = parseOptions(argc, argv);

        if (!std::filesystem::exists(options.dependencies_file)) {
            std::cerr << "File not found: " << options.dependencies_file << std::endl;
            return 1;
        }

        std::cout << "Checking for updates...\n";
        if (options.dry_run) {
            std::cout << "Running in dry-run mode - no files will be modified\n";
        }

        // Parse dependencies file
        auto [dependencies, content] = CMakeParser::parseDependencies(options.dependencies_file);
        if (dependencies.empty()) {
            std::cout << "No dependencies found in the specified file.\n";
            return 0;
        }

        // Check each dependency for updates
        bool updates_available = false;
        std::string title = "deps: Bump";
        std::string body;
        for (auto& dep : dependencies) {
            std::cout << "Checking " << dep.name << " (" << dep.repo << ")...\n";

            auto latest_version = GitHubAPI::getLatestVersion(dep.repo);
            if (!latest_version) {
                std::cout << "  Could not determine the latest version for " << dep.name
                         << " (current version: " << dep.tag << ")\n";
                continue;
            }

            dep.latest_version = *latest_version;
            auto current = GitHubAPI::versionToComponents(dep.tag);
            auto latest = GitHubAPI::versionToComponents(*latest_version);

            if (latest > current) {
                std::cout << "  Update available for " << dep.name << ": " << *latest_version << " (current: " << dep.tag << ")\n";
                if(updates_available) {
                    title += ",";
                }
                title += (" " + dep.name + " to " + *latest_version);
                body += ("Bump " + dep.name + " from " + dep.tag + " to " + *latest_version);
                updates_available = true;
            } else {
                std::cout << "  " << dep.name << " is up-to-date (current version: "
                         << dep.tag << ")\n";
            }
        }

        // Update the file if needed and not in dry-run mode
        if (updates_available) {
            if (!options.dry_run) {

                std::ofstream myfile;
                myfile.open ("title.log");
                myfile << title;
                myfile.close();

                myfile.open ("body.log");
                myfile << body;
                myfile.close();

                CMakeParser::updateFile(content, dependencies, options.dependencies_file);
                std::cout << "File updated successfully.\n";
            } else {
                std::cout << "Updates available but no changes made (dry-run mode).\n";
            }
        } else {
            std::cout << "No updates were made.\n";
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        printUsage(argv[0]);
        return 1;
    }
}
