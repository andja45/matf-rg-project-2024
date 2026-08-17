#ifndef SCENEOBJECT_HPP
#define SCENEOBJECT_HPP
#include <glm/glm.hpp>
#include <string>

namespace app {
class SceneObject {
    std::string m_model_name;
    glm::vec3 m_position = glm::vec3(0.0f);

    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    float m_roll = 0.0f;

    glm::vec3 m_scale = glm::vec3(1.0f);

    bool m_visible = true;

    float m_specular_strength = 1.0f;
    std::string m_shader_name = "blinn_phong";
    glm::vec3 m_emissive_color = glm::vec3(0.0f);

public:
    SceneObject(std::string model_name, glm::vec3 position, glm::vec3 scale,
                float yaw = 0.0f, float pitch = 0.0f, float roll = 0.0f,
                float specular_strength = 1.0f,
                std::string shader_name = "blinn_phong", glm::vec3 emissive_color = glm::vec3(0.0f));

    const std::string &model_name() const;

    glm::vec3 position() const;
    void set_position(glm::vec3 position);

    void set_emissive_color(glm::vec3 emissive_color);

    glm::mat4 model_matrix() const;

    bool visible() const;

    void set_visible(bool visible);

    const std::string &shader_name() const;
    glm::vec3 emissive_color() const;
    float specular_strength() const;
};
}// namespace app
#endif//SCENEOBJECT_HPP
