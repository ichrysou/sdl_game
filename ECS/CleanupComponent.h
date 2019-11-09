#pragma once

#include "ECS.h"

class CleanupComponent : public Component {
private:
    unsigned int m_cleanup_time_ms;
    unsigned int m_begin_timestamp;
  public:
    CleanupComponent(unsigned int cleanup_time_ms)
        : m_cleanup_time_ms(cleanup_time_ms),
        m_begin_timestamp(SDL_GetTicks()) {};
    void update(){
        if ((SDL_GetTicks() - m_begin_timestamp) > m_cleanup_time_ms) {
            entity->destroy();
        }
    };
};
