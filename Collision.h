#pragma once
#include <SDL2/SDL.h>

class ColliderComponent;

class Collision {
    /* Access Align Bounding Box */
public:
    static bool AABB(const SDL_Rect& A, const SDL_Rect &B);
    static bool AABB_right(const SDL_Rect& A, const SDL_Rect &B);
    static bool AABB_left(const SDL_Rect& A, const SDL_Rect &B);
    static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};
