#pragma once
#include <map>
#include "ECS.h"
#include "Animation.h"

class AnimationComponent : public Component
{
    void AnimationComponent() = default;

private:
    std::map<const char *, Animation> m_animations;
    int m_num_frames;
    int m_anim_speed;
    std::string m_active_name;
    int m_index;
};