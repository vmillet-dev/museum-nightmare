#include "GameObject.hpp"
#include <box2d/box2d.h>

namespace game {

GameObject::GameObject(float x, float y)
    : position(x, y), bodyId(b2_nullBodyId), worldId(b2_nullWorldId) {}

GameObject::~GameObject() {
    if (b2World_IsValid(worldId) && b2Body_IsValid(bodyId)) {
        b2DestroyBody(bodyId);
    }
}

const sf::Vector2f& GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    if (b2Body_IsValid(bodyId)) {
        b2Vec2 pos = {newPosition.x, newPosition.y};
        b2Rot rot = b2Body_GetRotation(bodyId);
        b2Transform xf = {pos, rot};
        b2Body_SetTransform(bodyId, xf.p, xf.q);
    }
}

void GameObject::syncPositionWithPhysics() {
    if (b2Body_IsValid(bodyId)) {
        b2Vec2 pos = b2Body_GetPosition(bodyId);
        position.x = pos.x;
        position.y = pos.y;
    }
}

} // namespace game
