// clang-format off
#include <glad/glad.h>
// clang-format on
#include <engine/graphics/OpenGL.hpp>
#include <engine/graphics/PointShadowFramebuffer.hpp>
#include <engine/util/Errors.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine::graphics {
    void PointShadowFramebuffer::init(int size) {
        m_size = size;

        CHECKED_GL_CALL(glGenTextures, 1, &m_depth_cubemap);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, m_depth_cubemap);
        for (int i = 0; i < 6; i++) {
            CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, size, size, 0,
                            GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        CHECKED_GL_CALL(glGenFramebuffers, 1, &m_fbo);
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_fbo);
        CHECKED_GL_CALL(glFramebufferTexture, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth_cubemap, 0);
        CHECKED_GL_CALL(glDrawBuffer, GL_NONE);
        CHECKED_GL_CALL(glReadBuffer, GL_NONE);

        if (CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            RG_ENGINE_ERROR(engine::util::EngineError::Type::OpenGLError, "Incomplete framebuffer");
        }
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    }

    PointShadowFramebuffer::~PointShadowFramebuffer() {
        CHECKED_GL_CALL(glDeleteTextures, 1, &m_depth_cubemap);
        CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_fbo);
    }

    void PointShadowFramebuffer::bind() const {
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_fbo);
        CHECKED_GL_CALL(glViewport, 0, 0, m_size, m_size);
        CHECKED_GL_CALL(glClear, GL_DEPTH_BUFFER_BIT);
    }

    void PointShadowFramebuffer::unbind(int window_width, int window_height) {
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
        CHECKED_GL_CALL(glViewport, 0, 0, window_width, window_height);
    }

    uint32_t PointShadowFramebuffer::depth_cubemap_id() const {
        return m_depth_cubemap;
    }

    std::array<glm::mat4, 6> PointShadowFramebuffer::calculate_shadow_matrices(glm::vec3 light_position,
                                                                               float near_plane, float far_plane,
                                                                               int shadow_width, int shadow_height) {
        glm::mat4 shadow_projection = glm::perspective(glm::radians(90.0f),
                                                       static_cast<float>(shadow_width) / static_cast<float>(
                                                           shadow_height), near_plane, far_plane);

        return {
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(1.0f, 0.0f, 0.0f),
                                            glm::vec3(0.0f, -1.0f, 0.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(-1.0f, 0.0f, 0.0f),
                                            glm::vec3(0.0f, -1.0f, 0.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0.0f, 1.0f, 0.0f),
                                            glm::vec3(0.0f, 0.0f, 1.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0.0f, -1.0f, 0.0f),
                                            glm::vec3(0.0f, 0.0f, -1.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0.0f, 0.0f, 1.0f),
                                            glm::vec3(0.0f, -1.0f, 0.0f)),
            shadow_projection * glm::lookAt(light_position, light_position + glm::vec3(0.0f, 0.0f, -1.0f),
                                            glm::vec3(0.0f, -1.0f, 0.0f)),
        };
    }
} // namespace engine::graphics
