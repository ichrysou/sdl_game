#pragma once
#include <map>
#include "Animation.h"
#include "ECS.h"

class AnimationComponent : public Component {
   public:
    AnimationComponent(std::string name) : m_name(name), m_active(NULL) {}

   private:
    std::map<std::string, Animation> m_animations;
    std::string m_name;  // name of the sprite
    std::string m_active_name;
    Animation *m_active;
    int m_frame_timestamp = 0;
    int m_time_delta = 0;
    int m_frameno = 0;

    /* int m_num_frames;
    int m_anim_speed;
    std::string m_active_name; // name of the specific animation for a sprite
    int m_index;
    double m_angle; */

   public:
    void addAnimation(Animation anim) {
        m_animations[anim.getName()] = anim;
        if (!m_active) {
            m_active = &m_animations[anim.getName()];
            m_active_name = anim.getName();
        }
    }
    void setActive(std::string name) {
        if (m_animations.find(name) != m_animations.end())
            m_active = &m_animations[name];
        else
            std::cout << "animation not found" << std::endl;
    }
    Animation *getActive() { return m_active; }
    void update() {
        auto anim = getActive();
        if (m_active_name != anim->getName()) {
            m_frameno = 0;
            m_active_name = anim->getName();
        }
        anim->setCurrentFrame(m_frameno);
        auto sprite = &entity->getComponent<SpriteComponent>();
        sprite->setSrcXY(anim->m_start_x + anim->getCurrentFrame() * anim->m_frame_w, anim->m_start_y);
        m_time_delta = SDL_GetTicks() - m_frame_timestamp;
        if (m_time_delta >= anim->m_speed) {
            m_frameno = (m_frameno + 1) % anim->m_num_frames;
            m_frame_timestamp = SDL_GetTicks();
        }
    };
};
