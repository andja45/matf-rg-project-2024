#include <SceneObject.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace app {
SceneObject::SceneObject(std::string model_name, glm::vec3 position, glm::vec3 scale,
                         float yaw, float pitch, float roll)
    : m_model_name(std::move(model_name))
    , m_position(position)
    , m_yaw(yaw)
    , m_pitch(pitch)
    , m_roll(roll)
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
    model = glm::rotate(model, glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_roll), glm::vec3(0.0f, 0.0f, 1.0f));
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
