#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP
#include <engine/graphics/Light.hpp>

namespace engine::graphics {
class PointLight final : public Light {
    glm::vec3 m_position;
    float m_constant;
    float m_linear;
    float m_quadratic;

public:
    PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient,
               glm::vec3 diffuse, glm::vec3 specular);

    void set_uniforms(resources::Shader *shader, int index) const override;

    const glm::vec3 &position() const;

    void set_position(glm::vec3 position);
};
}// namespace engine::graphics
#endif//POINTLIGHT_HPP
