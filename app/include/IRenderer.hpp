#ifndef IRENDERER_HPP
#define IRENDERER_HPP
#include <Light.hpp>
#include <SceneObject.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace app {
enum class RendererType { Deferred,
                          ForwardPlus };

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void initialize(uint32_t width, uint32_t height) = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual void render(const std::vector<SceneObject *> &lit_objects,
                        const DirectionalLight &dir_light, const PointLight &point_light,
                        const glm::mat4 &projection, const glm::mat4 &view, glm::vec3 view_pos,
                        uint32_t point_shadow_cubemap, float point_shadow_far_plane,
                        bool point_shadows_enabled, uint32_t target_fbo) = 0;
};
}// namespace app
#endif//IRENDERER_HPP
