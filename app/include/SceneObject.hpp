#ifndef SCENEOBJECT_HPP
#define SCENEOBJECT_HPP
#include <glm/glm.hpp>
#include <string>

namespace app {
class SceneObject {
    std::string m_model_name;
    glm::vec3 m_position = glm::vec3(0.0f);

    glm::vec3 m_rotation_axis = glm::vec3(0.0f, 1.0f, 0.0f);
    float m_rotation_angle = 0.0f;
    glm::vec3 m_scale = glm::vec3(1.0f);

    bool m_visible = true;

public:
    SceneObject(std::string model_name, glm::vec3 position, glm::vec3 scale);

    const std::string &model_name() const;

    glm::vec3 position() const;

    glm::mat4 model_matrix() const;

    bool visible() const;

    void set_visible(bool visible);
};
}// namespace app
#endif//SCENEOBJECT_HPP
