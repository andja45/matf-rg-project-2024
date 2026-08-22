#ifndef POINTSHADOWFRAMEBUFFER_HPP
#define POINTSHADOWFRAMEBUFFER_HPP
#include <array>
#include <cstdint>
#include <glm/glm.hpp>

namespace engine::graphics {
class PointShadowFramebuffer {
    uint32_t m_fbo = 0;
    uint32_t m_depth_cubemap = 0;
    int m_size = 0;

public:
    void init(int size);

    void destroy();

    void bind() const;

    static void unbind(int window_width, int window_height);// return viewport to windows real size

    uint32_t depth_cubemap_id() const;

    static std::array<glm::mat4, 6> calculate_shadow_matrices(glm::vec3 light_position, float near_plane,
                                                              float far_plane, int shadow_width, int shadow_height);
};
}// namespace engine::graphics
#endif//POINTSHADOWFRAMEBUFFER_HPP
