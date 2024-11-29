#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

namespace game {

class TextureManager {
public:
    static TextureManager& getInstance() {
        static TextureManager instance;
        return instance;
    }

    const sf::Texture& getTexture(const std::string& filename);
    void clearTextures();

    // Delete copy constructor and assignment operator
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

private:
    TextureManager() = default;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
};

} // namespace game
