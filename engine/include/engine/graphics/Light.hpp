#ifndef LIGHT_HPP
#define LIGHT_HPP
#include <glm/vec3.hpp>

namespace engine::resources {
class Shader;
}

namespace engine::graphics {
class Light {
protected:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_intensity = 1.0f;

public:
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    virtual ~Light() = default;

    virtual void set_uniforms(resources::Shader *shader, int index) const = 0;

    float intensity() const;

    void set_intensity(float intensity);
};
}// namespace engine::graphics
#endif//LIGHT_HPP
