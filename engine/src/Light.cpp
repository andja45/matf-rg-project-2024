#include <engine/graphics/Light.hpp>

namespace engine::graphics {
Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : m_ambient(ambient)
    , m_diffuse(diffuse)
    , m_specular(specular) {
}

float Light::intensity() const {
    return m_intensity;
}

void Light::set_intensity(float intensity) {
    m_intensity = intensity;
}
}// namespace engine::graphics
