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

public:
    SceneObject(std::string model_name, glm::vec3 position, glm::vec3 scale,
                float yaw = 0.0f, float pitch = 0.0f, float roll = 0.0f);

    const std::string &model_name() const;

    glm::vec3 position() const;

    glm::mat4 model_matrix() const;

    bool visible() const;

    void set_visible(bool visible);
};
}// namespace app
#endif//SCENEOBJECT_HPP
