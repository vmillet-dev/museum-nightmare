#pragma once
#include <box2d/box2d.h>
#include "../objects/GameObject.hpp"
#include <spdlog/spdlog.h>

namespace game {

class ContactListener {
public:
    static void processContactEvents(b2WorldId worldId) {
        b2ContactEvents events = b2World_GetContactEvents(worldId);

        // Process begin contacts
        for (int32_t i = 0; i < events.beginCount; ++i) {
            const b2ContactBeginTouchEvent& event = events.beginEvents[i];
            handleContact(worldId, event.shapeIdA, event.shapeIdB);
        }

        // Process hit contacts (significant impacts)
        for (int32_t i = 0; i < events.hitCount; ++i) {
            const b2ContactHitEvent& event = events.hitEvents[i];
            handleContact(worldId, event.shapeIdA, event.shapeIdB);
        }
    }

private:
    static void handleContact(b2WorldId worldId, b2ShapeId shapeIdA, b2ShapeId shapeIdB) {
        // Get the bodies from the shapes
        b2BodyId bodyIdA = b2Shape_GetBody(shapeIdA);
        b2BodyId bodyIdB = b2Shape_GetBody(shapeIdB);

        // Get user data from bodies
        void* userDataA = b2Body_GetUserData(bodyIdA);
        void* userDataB = b2Body_GetUserData(bodyIdB);

        GameObject* objectA = static_cast<GameObject*>(userDataA);
        GameObject* objectB = static_cast<GameObject*>(userDataB);

        if (objectA && objectB) {
            spdlog::debug("Collision between objects at positions ({},{}) and ({},{})",
                objectA->getPosition().x, objectA->getPosition().y,
                objectB->getPosition().x, objectB->getPosition().y);
        }
    }
};

} // namespace game
