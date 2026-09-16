#ifndef DEFERREDRENDERER_HPP
#define DEFERREDRENDERER_HPP
#include <IRenderer.hpp>
#include <engine/graphics/GBuffer.hpp>
#include <memory>

namespace app {
class DeferredRenderer final : public IRenderer {
    std::unique_ptr<engine::graphics::GBuffer> m_gbuffer;
    uint32_t m_width = 0;
    uint32_t m_height = 0;

public:
    ~DeferredRenderer() override;

    void initialize(uint32_t width, uint32_t height) override;

    void resize(uint32_t width, uint32_t height) override;

    void render(const std::vector<SceneObject *> &lit_objects,
                const DirectionalLight &dir_light, const PointLight &point_light,
                const glm::mat4 &projection, const glm::mat4 &view, glm::vec3 view_pos,
                uint32_t point_shadow_cubemap, float point_shadow_far_plane, bool point_shadows_enabled,
                uint32_t target_fbo) override;
};
}// namespace app
#endif//DEFERREDRENDERER_HPP
