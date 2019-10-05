#pragma once

struct Frame
{
    int m_x;
    int m_y;
    int m_w;
    int m_h;
};

struct Animation
{
    std::string m_name;
    std::string m_tex_id;
    std::vector<Frame> m_frames;
    int m_num_frames;
    int m_speed;
    int m_start_x = 0;
    int m_start_y = 0;
    int m_frame_w = 0;
    int m_frame_h = 0;
    Animation() {}
    Animation(int f, int s)
    {
        m_num_frames = f;
        m_speed = s;
    };
    Animation(std::string name, std::string tex_id, int frames, int speed, int start_x, int start_y, int frame_w, int frame_h)
    {
        m_name = name;
        m_tex_id = tex_id;
        m_num_frames = frames;
        m_speed = speed;
        m_start_x = start_x;
        m_start_y = start_y;
        m_frame_w = frame_w;
        m_frame_h = frame_h;
    };
    std::string getName() { return m_name; };
};