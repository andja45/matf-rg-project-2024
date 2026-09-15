// clang-format off
#include <glad/glad.h>
// clang-format on
#include <engine/graphics/GBuffer.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/util/Errors.hpp>

namespace engine::graphics {
static uint32_t create_color_target(uint32_t width, uint32_t height, int32_t internal_format, uint32_t format,
                                    uint32_t attachment) {
    uint32_t id = 0;
    CHECKED_GL_CALL(glGenTextures, 1, &id);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, id);
    CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_FLOAT, nullptr);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    CHECKED_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id, 0);
    return id;
}

GBuffer::GBuffer(uint32_t width, uint32_t height) {
    CHECKED_GL_CALL(glGenFramebuffers, 1, &m_fbo);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_fbo);

    m_position = create_color_target(width, height, GL_RGBA16F, GL_RGBA, GL_COLOR_ATTACHMENT0);
    m_normal_spec = create_color_target(width, height, GL_RGBA16F, GL_RGBA, GL_COLOR_ATTACHMENT1);
    m_albedo = create_color_target(width, height, GL_RGBA8, GL_RGBA, GL_COLOR_ATTACHMENT2);
    m_emissive = create_color_target(width, height, GL_RGBA16F, GL_RGBA, GL_COLOR_ATTACHMENT3);

    uint32_t attachments[4] = {
            GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3};
    CHECKED_GL_CALL(glDrawBuffers, 4, attachments);

    CHECKED_GL_CALL(glGenRenderbuffers, 1, &m_depth_rbo);
    CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, m_depth_rbo);
    CHECKED_GL_CALL(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    CHECKED_GL_CALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_rbo);

    if (CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        RG_ENGINE_ERROR(engine::util::EngineError::Type::OpenGLError, "Incomplete framebuffer");
    }
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
}

void GBuffer::destroy() {
    CHECKED_GL_CALL(glDeleteTextures, 1, &m_position);
    CHECKED_GL_CALL(glDeleteTextures, 1, &m_normal_spec);
    CHECKED_GL_CALL(glDeleteTextures, 1, &m_albedo);
    CHECKED_GL_CALL(glDeleteTextures, 1, &m_emissive);
    CHECKED_GL_CALL(glDeleteRenderbuffers, 1, &m_depth_rbo);
    CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_fbo);
}

void GBuffer::bind() const {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_fbo);
    OpenGL::clear_buffers();
}

uint32_t GBuffer::fbo_id() const {
    return m_fbo;
}

void GBuffer::bind_textures(uint32_t position_unit, uint32_t normal_spec_unit, uint32_t albedo_unit,
                            uint32_t emissive_unit) const {
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0 + position_unit);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_position);
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0 + normal_spec_unit);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_normal_spec);
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0 + albedo_unit);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_albedo);
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0 + emissive_unit);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_emissive);
}
}// namespace engine::graphics
