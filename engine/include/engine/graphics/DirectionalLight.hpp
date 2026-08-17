#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP
#include <engine/graphics/Light.hpp>

namespace engine::graphics {
    class DirectionalLight final : public Light {
        glm::vec3 m_direction;

    public:
        DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

        void set_uniforms(resources::Shader *shader, int index) const override;
    };
} // namespace engine::graphics
#endif//DIRECTIONALLIGHT_HPP
