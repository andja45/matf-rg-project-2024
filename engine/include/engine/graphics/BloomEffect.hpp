#ifndef BLOOMEFFECT_HPP
#define BLOOMEFFECT_HPP
#include <cstdint>

namespace engine::resources {
    class Shader;
}

namespace engine::graphics {
    class BloomEffect {
        uint32_t m_scene_fbo          = 0;
        uint32_t m_scene_color_buffer = 0;
        uint32_t m_scene_depth_rbo    = 0;

        uint32_t m_bright_fbo          = 0;
        uint32_t m_bright_color_buffer = 0;

        uint32_t m_ping_pong_fbo[2]           = {0, 0};
        uint32_t m_ping_pong_color_buffers[2] = {0, 0};

        uint32_t m_quad_vao = 0;
        uint32_t m_quad_vbo = 0;

        void render_quad();

    public:
        void init(uint32_t width, uint32_t height);

        ~BloomEffect();

        void begin_scene_capture();

        void apply(resources::Shader *extract_shader, resources::Shader *blur_shader,
                   resources::Shader *combine_shader, bool bloom_enabled);
    };
} // namespace engine::graphics
#endif//BLOOMEFFECT_HPP
