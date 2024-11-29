#include "TextureManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

const sf::Texture& TextureManager::getTexture(const std::string& filename) {
    auto it = textures.find(filename);
    if (it != textures.end()) {
        return *it->second;
    }

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filename)) {
        spdlog::error("Failed to load texture: {}", filename);
        throw std::runtime_error("Failed to load texture: " + filename);
    }

    const sf::Texture& textureRef = *texture;
    textures.emplace(filename, std::move(texture));
    return textureRef;
}

void TextureManager::clearTextures() {
    textures.clear();
    spdlog::debug("Cleared all cached textures");
}

} // namespace game
