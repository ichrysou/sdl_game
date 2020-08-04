#pragma once

struct Frame {
    int m_x;
    int m_y;
    int m_w;
    int m_h;
};

struct Animation {
    std::string m_name;
    std::string m_tex_id;
    std::vector<Frame> m_frames;
    int m_num_frames;
    int m_current_frame = 0;
    int m_speed;
    int m_start_x = 0;
    int m_start_y = 0;
    int m_frame_w = 0;
    int m_frame_h = 0;
    bool m_loop;
    bool m_done = false;
    bool m_played_last = false;
    Animation() {}
    Animation(int f, int s) {
        m_num_frames = f;
        m_speed = s;
    };
    Animation(std::string name, std::string tex_id, int frames, int speed, int start_x, int start_y, int frame_w, int frame_h,
              bool loop = true) {
        m_name = name;
        m_tex_id = tex_id;
        m_num_frames = frames;
        m_speed = speed;
        m_start_x = start_x;
        m_start_y = start_y;
        m_frame_w = frame_w;
        m_frame_h = frame_h;
        m_loop = loop;
    };
    std::string getName() { return m_name; };
    void reset() { m_done = false; m_played_last = false; };
    int getCurrentFrame() { return m_current_frame; };
    void changeFrame() {
        int frameno = (SDL_GetTicks() / m_speed) % m_num_frames;  // actual
        setCurrentFrame(frameno);
    };
    bool isDone() { return m_done; };
    bool isLoop() { return m_loop; };
    void setCurrentFrame(int frameno) {
        if (frameno == m_num_frames - 1) {
            m_played_last = true;
        }
        if (!m_loop) {
            if (m_done) return;
            if (m_played_last) m_done = true;
        }
        m_current_frame = frameno;
    };
};
