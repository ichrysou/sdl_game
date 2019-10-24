#pragma once
#include <map>
#include "ECS.h"
#include "Animation.h"

class AnimationComponent : public Component
{
public:
    AnimationComponent(std::string name) : m_name(name), m_active(NULL)
    {
    }

private:
    std::map<std::string, Animation> m_animations;
    std::string m_name; //name of the sprite
    std::string m_active_name;
    Animation *m_active;
    /* int m_num_frames;
    int m_anim_speed;
    std::string m_active_name; // name of the specific animation for a sprite
    int m_index;
    double m_angle; */

public:
    void addAnimation(Animation anim)
    {
        m_animations[anim.getName()] = anim;
        if (!m_active)
            m_active = &m_animations[anim.getName()];
    }
    void setActive(std::string name)
    {
        if (m_animations.find(name) != m_animations.end())
            m_active = &m_animations[name];
        else
            std::cout << "animation not found" << std::endl;
    }
    Animation *getActive()
    {
        return m_active;
    }
};