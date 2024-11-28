#ifndef GIDMAPPER_HPP
#define GIDMAPPER_HPP

#include "../objects/GameObject.hpp"
#include "../objects/FloorObject.hpp"
#include "../objects/ParallaxBackground.hpp"
#include <memory>
#include <map>

namespace game {

class GIDMapper {
public:
    GIDMapper();

    // Register GID ranges for different object types
    void registerGIDRange(unsigned int firstGid, unsigned int lastGid, const std::string& objectType);

    // Create appropriate GameObject based on GID
    std::unique_ptr<game::GameObject> createGameObjectFromGID(unsigned int gid, const sf::Texture& texture, const sf::Vector2f& position);

private:
    struct GIDRange {
        unsigned int firstGid;
        unsigned int lastGid;
        std::string objectType;
    };

    std::vector<GIDRange> gidRanges;
};

} // namespace game

#endif // GIDMAPPER_HPP
