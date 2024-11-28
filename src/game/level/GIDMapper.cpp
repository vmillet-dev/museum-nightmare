#include "GIDMapper.hpp"
#include <stdexcept>

namespace game {

GIDMapper::GIDMapper() {}

void GIDMapper::registerGIDRange(unsigned int firstGid, unsigned int lastGid, const std::string& objectType) {
    gidRanges.push_back({firstGid, lastGid, objectType});
}

std::unique_ptr<game::GameObject> GIDMapper::createGameObjectFromGID(unsigned int gid, const sf::Texture& texture, const sf::Vector2f& position) {
    // Find the matching GID range
    for (const auto& range : gidRanges) {
        if (gid >= range.firstGid && gid <= range.lastGid) {
            if (range.objectType == "ParallaxBackground") {
                // Calculate depth based on GID (example: 0.01 for far, 0.98 for middle, 0.94 for near)
                float depth = (gid == range.firstGid) ? 0.01f :
                            (gid == range.firstGid + 1) ? 0.98f : 0.94f;
                return std::make_unique<game::ParallaxBackground>(texture, position, depth);
            }
            else if (range.objectType == "Floor") {
                // Create a default texture rect for the floor object
                sf::IntRect textureRect(0, 0, texture.getSize().x, texture.getSize().y);
                return std::make_unique<game::FloorObject>(texture, textureRect, position, b2_nullWorldId);
            }
        }
    }

    throw std::runtime_error("Invalid GID: " + std::to_string(gid));
}

} // namespace game
