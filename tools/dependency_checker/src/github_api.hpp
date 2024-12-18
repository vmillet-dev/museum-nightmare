#pragma once
#include <string>
#include <optional>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>

class GitHubAPI {
public:
    // Get the latest version tag from a GitHub repository
    static std::optional<std::string> getLatestVersion(const std::string& repo_url);

    // Convert version string to comparable components
    static std::vector<int> versionToComponents(const std::string& version);

private:
    // Parse owner and repo from GitHub URL
    static std::pair<std::string, std::string> parseGitHubUrl(const std::string& url);

    // Make HTTP request to GitHub API
    static std::string makeRequest(const std::string& url);
};
