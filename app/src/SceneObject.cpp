#include <SceneObject.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace app {
SceneObject::SceneObject(std::string model_name, glm::vec3 position, glm::vec3 scale)
    : m_model_name(std::move(model_name))
    , m_position(position)
    , m_scale(scale) {
}

const std::string &SceneObject::model_name() const {
    return m_model_name;
}

glm::vec3 SceneObject::position() const {
    return m_position;
}

glm::mat4 SceneObject::model_matrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::rotate(model, m_rotation_angle, m_rotation_axis);
    model = glm::scale(model, m_scale);
    return model;
}

bool SceneObject::visible() const {
    return m_visible;
}

void SceneObject::set_visible(bool visible) {
    m_visible = visible;
}
}// namespace app
