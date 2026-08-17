#include <engine/graphics/PointLight.hpp>
#include <engine/resources/Shader.hpp>
#include <string>

namespace engine::graphics {
    PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient,
                           glm::vec3 diffuse, glm::vec3 specular)
    : Light(ambient, diffuse, specular)
  , m_position(position)
  , m_constant(constant)
  , m_linear(linear)
  , m_quadratic(quadratic) {
    }

    void PointLight::set_uniforms(resources::Shader *shader, int index) const {
        const std::string prefix = "pointLights[" + std::to_string(index) + "].";
        shader->set_vec3(prefix + "position", m_position);
        shader->set_float(prefix + "constant", m_constant);
        shader->set_float(prefix + "linear", m_linear);
        shader->set_float(prefix + "quadratic", m_quadratic);
        shader->set_vec3(prefix + "ambient", m_ambient * intensity());
        shader->set_vec3(prefix + "diffuse", m_diffuse * intensity());
        shader->set_vec3(prefix + "specular", m_specular * intensity());
    }

    const glm::vec3 &PointLight::position() const {
        return m_position;
    }
} // namespace engine::graphics
