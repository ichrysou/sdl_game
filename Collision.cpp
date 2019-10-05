#include "Collision.h"
#include "ECS/ColliderComponent.h"
bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
    if (
        recA.x + recA.w >= recB.x &&
        recB.x + recB.w >= recA.x &&
        recA.y + recA.h >= recB.y &&
        recB.y + recB.h >= recA.y)
    {
        return true;
    }
    return false;
}
//TODO: collision with the wall should still allow the sprite to move sideways
/* bool Collision::AABB_right(const SDL_Rect& recA, const SDL_Rect& recB) {
    if (
        recA.x + recA.w >= recB.x  &&
        recA.x < recB.x
        // recB.x + recB.w >= recA.x &&
        // recA.y + recA.h >= recB.y &&
        // recB.y + recB.h >= recA.y
        )
    {
        return true;
    }
    return false;
}

bool Collision::AABB_left(const SDL_Rect& recA, const SDL_Rect& recB) {
    if (
        // recA.x + recA.w >= recB.x &&
        recB.x + recB.w >= recA.x  &&
        recB.x <= recA.x
        // recA.y + recA.h >= recB.y &&
        // recB.y + recB.h >= recA.y
        )
    {
        return true;
    }
    return false;
}
*/
bool Collision::AABB(const ColliderComponent &colA, const ColliderComponent &colB) {
    if (AABB(colA.collider, colB.collider)) {
        //std::cout << colA.tag << " hit: " << colB.tag << std::endl;
        return true;
    } else {
        return false;
    }
}
