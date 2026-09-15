#ifndef GBUFFER_HPP
#define GBUFFER_HPP
#include <cstdint>

namespace engine::graphics {
class GBuffer {
    uint32_t m_fbo = 0;
    uint32_t m_position = 0;
    uint32_t m_normal_spec = 0;
    uint32_t m_albedo = 0;
    uint32_t m_emissive = 0;
    uint32_t m_depth_rbo = 0;

public:
    GBuffer(uint32_t width, uint32_t height);

    void destroy();

    void bind() const;

    uint32_t fbo_id() const;

    void bind_textures(uint32_t position_unit, uint32_t normal_spec_unit, uint32_t albedo_unit,
                       uint32_t emissive_unit) const;
};
}// namespace engine::graphics
#endif//GBUFFER_HPP
