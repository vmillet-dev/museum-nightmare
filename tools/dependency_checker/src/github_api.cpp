#include "github_api.hpp"
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cstdlib>

using json = nlohmann::json;

// Callback for CURL to write response data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::pair<std::string, std::string> GitHubAPI::parseGitHubUrl(const std::string& url) {
    std::regex pattern(R"(github\.com[:/]([^/]+)/([^/\.]+))");
    std::smatch matches;
    if (std::regex_search(url, matches, pattern) && matches.size() > 2) {
        return {matches[1].str(), matches[2].str()};
    }
    throw std::runtime_error("Invalid GitHub URL format");
}

std::string GitHubAPI::makeRequest(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }

    std::string response_data;
    struct curl_slist* headers = nullptr;

    // Add GitHub token if available
    const char* github_token = std::getenv("GITHUB_TOKEN");
    if (github_token) {
        std::string auth_header = "Authorization: Bearer " + std::string(github_token);
        headers = curl_slist_append(headers, auth_header.c_str());
        std::cout << "Using GitHub token for authentication" << std::endl;
    } else {
        std::cout << "Warning: No GitHub token found in environment" << std::endl;
    }

    headers = curl_slist_append(headers, "Accept: application/vnd.github.v3+json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dependency-Checker/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::cout << "Making request to: " << url << std::endl;

    CURLcode res = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        throw std::runtime_error("Failed to make HTTP request: " + std::string(curl_easy_strerror(res)));
    }

    if (http_code != 200) {
        throw std::runtime_error("GitHub API request failed with status code: " + std::to_string(http_code));
    }

    return response_data;
}

std::vector<int> GitHubAPI::versionToComponents(const std::string& version) {
    std::vector<int> components;
    std::string version_str = version;
    if (version_str[0] == 'v') {
        version_str = version_str.substr(1);
    }

    std::regex number_pattern(R"(\d+)");
    auto numbers_begin = std::sregex_iterator(version_str.begin(), version_str.end(), number_pattern);
    auto numbers_end = std::sregex_iterator();

    for (std::sregex_iterator i = numbers_begin; i != numbers_end; ++i) {
        components.push_back(std::stoi(i->str()));
    }

    return components;
}

std::optional<std::string> GitHubAPI::getLatestVersion(const std::string& repo_url) {
    try {
        auto [owner, repo] = parseGitHubUrl(repo_url);
        std::string api_url = "https://api.github.com/repos/" + owner + "/" + repo + "/tags";

        std::string response = makeRequest(api_url);
        auto tags = json::parse(response);

        std::optional<std::string> latest_version;
        std::vector<int> highest_version;

        for (const auto& tag : tags) {
            std::string tag_name = tag["name"];
            if (std::regex_match(tag_name, std::regex(R"(v?\d+\.\d+\.\d+$)"))) {
                auto version_components = versionToComponents(tag_name);

                if (latest_version.has_value()) {
                    auto current_highest = versionToComponents(latest_version.value());
                    if (version_components > current_highest) {
                        latest_version = tag_name;
                        highest_version = version_components;
                    }
                } else {
                    latest_version = tag_name;
                    highest_version = version_components;
                }
            }
        }

        if (latest_version) {
            std::cout << "Latest version found: " << latest_version.value() << std::endl;
        } else {
            std::cout << "No valid version tags found" << std::endl;
        }

        return latest_version;
    } catch (const std::exception& e) {
        std::cerr << "Error getting latest version: " << e.what() << std::endl;
        return std::nullopt;
    }
}
