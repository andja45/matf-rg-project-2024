// clang-format off
#include <glad/glad.h>
// clang-format on
#include <engine/graphics/BloomEffect.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Shader.hpp>
#include <engine/util/Errors.hpp>

namespace engine::graphics {
    static uint32_t create_hdr_color_buffer(uint32_t width, uint32_t height) {
        uint32_t id = 0;
        CHECKED_GL_CALL(glGenTextures, 1, &id);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, id);
        CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return id;
    }

    void BloomEffect::init(uint32_t width, uint32_t height) {
        CHECKED_GL_CALL(glGenFramebuffers, 1, &m_scene_fbo);
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_scene_fbo);

        m_scene_color_buffer = create_hdr_color_buffer(width, height);
        CHECKED_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                        m_scene_color_buffer, 0);

        CHECKED_GL_CALL(glGenRenderbuffers, 1, &m_scene_depth_rbo);
        CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, m_scene_depth_rbo);
        CHECKED_GL_CALL(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        CHECKED_GL_CALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                        m_scene_depth_rbo);

        if (CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            RG_ENGINE_ERROR(engine::util::EngineError::Type::OpenGLError, "Incomplete framebuffer");
        }

        CHECKED_GL_CALL(glGenFramebuffers, 1, &m_bright_fbo);
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_bright_fbo);
        m_bright_color_buffer = create_hdr_color_buffer(width, height);
        CHECKED_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                        m_bright_color_buffer, 0);

        if (CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            RG_ENGINE_ERROR(engine::util::EngineError::Type::OpenGLError, "Incomplete framebuffer");
        }

        CHECKED_GL_CALL(glGenFramebuffers, 2, m_ping_pong_fbo);
        for (int i = 0; i < 2; i++) {
            CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_ping_pong_fbo[i]);
            m_ping_pong_color_buffers[i] = create_hdr_color_buffer(width, height);
            CHECKED_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                            m_ping_pong_color_buffers[i], 0);

            if (CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                RG_ENGINE_ERROR(engine::util::EngineError::Type::OpenGLError, "Incomplete framebuffer");
            }
        }

        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

        float quad_vertices[] = {
            -1.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, 1.0f, 1.0f, 1.0f,   // top-right
            1.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
        };
        CHECKED_GL_CALL(glGenVertexArrays, 1, &m_quad_vao);
        CHECKED_GL_CALL(glGenBuffers, 1, &m_quad_vbo);
        CHECKED_GL_CALL(glBindVertexArray, m_quad_vao);
        CHECKED_GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_quad_vbo);
        CHECKED_GL_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
        CHECKED_GL_CALL(glEnableVertexAttribArray, 0);
        CHECKED_GL_CALL(glVertexAttribPointer, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        CHECKED_GL_CALL(glEnableVertexAttribArray, 1);
        CHECKED_GL_CALL(glVertexAttribPointer, 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *) (2 * sizeof(float)));
        CHECKED_GL_CALL(glBindVertexArray, 0);
    }

    BloomEffect::~BloomEffect() {
        CHECKED_GL_CALL(glDeleteTextures, 1, &m_scene_color_buffer);
        CHECKED_GL_CALL(glDeleteRenderbuffers, 1, &m_scene_depth_rbo);
        CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_scene_fbo);

        CHECKED_GL_CALL(glDeleteTextures, 1, &m_bright_color_buffer);
        CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_bright_fbo);

        CHECKED_GL_CALL(glDeleteTextures, 2, m_ping_pong_color_buffers);
        CHECKED_GL_CALL(glDeleteFramebuffers, 2, m_ping_pong_fbo);

        CHECKED_GL_CALL(glDeleteVertexArrays, 1, &m_quad_vao);
        CHECKED_GL_CALL(glDeleteBuffers, 1, &m_quad_vbo);
    }

    void BloomEffect::render_quad() {
        CHECKED_GL_CALL(glBindVertexArray, m_quad_vao);
        CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLE_STRIP, 0, 4);
        CHECKED_GL_CALL(glBindVertexArray, 0);
    }

    void BloomEffect::begin_scene_capture() {
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_scene_fbo);
        OpenGL::clear_buffers();
    }

    void BloomEffect::apply(resources::Shader *extract_shader, resources::Shader *blur_shader,
                            resources::Shader *combine_shader, bool bloom_enabled) {
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_bright_fbo);
        OpenGL::clear_buffers();
        extract_shader->use();
        CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_scene_color_buffer);
        extract_shader->set_int("sceneTexture", 0);
        render_quad();

        bool horizontal           = true;
        bool first_iteration      = true;
        constexpr int BLUR_PASSES = 10;
        blur_shader->use();
        for (int i = 0; i < BLUR_PASSES; i++) {
            CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_ping_pong_fbo[horizontal]);
            blur_shader->set_bool("horizontal", horizontal);
            CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
            CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D,
                            first_iteration ? m_bright_color_buffer : m_ping_pong_color_buffers[!horizontal]);
            blur_shader->set_int("image", 0);
            render_quad();
            horizontal      = !horizontal;
            first_iteration = false;
        }

        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
        OpenGL::clear_buffers();
        combine_shader->use();
        CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_scene_color_buffer);
        combine_shader->set_int("sceneTexture", 0);
        CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE1);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_ping_pong_color_buffers[!horizontal]);
        combine_shader->set_int("bloomTexture", 1);
        combine_shader->set_bool("bloom", bloom_enabled);
        render_quad();
    }
} // namespace engine::graphics
