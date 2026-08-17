#include <engine/graphics/DirectionalLight.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::graphics {
    DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(ambient, diffuse, specular)
  , m_direction(direction) {
    }

    void DirectionalLight::set_uniforms(resources::Shader *shader, int) const {
        shader->set_vec3("dirLight.direction", m_direction);
        shader->set_vec3("dirLight.ambient", m_ambient * intensity());
        shader->set_vec3("dirLight.diffuse", m_diffuse * intensity());
        shader->set_vec3("dirLight.specular", m_specular * intensity());
    }
} // namespace engine::graphics
