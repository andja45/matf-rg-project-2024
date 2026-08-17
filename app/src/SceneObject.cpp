#include <SceneObject.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace app {
    SceneObject::SceneObject(std::string model_name, glm::vec3 position, glm::vec3 scale,
                             float yaw, float pitch, float roll,
                             float specular_strength,
                             std::string shader_name, glm::vec3 emissive_color)
    : m_model_name(std::move(model_name))
  , m_position(position)
  , m_yaw(yaw)
  , m_pitch(pitch)
  , m_roll(roll)
  , m_scale(scale)
  , m_specular_strength(specular_strength)
  , m_shader_name(std::move(shader_name))
  , m_emissive_color(emissive_color) {
    }

    const std::string &SceneObject::model_name() const {
        return m_model_name;
    }

    glm::vec3 SceneObject::position() const {
        return m_position;
    }

    void SceneObject::set_position(glm::vec3 position) {
        m_position = position;
    }

    glm::mat4 SceneObject::model_matrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, m_position);
        model           = glm::rotate(model, glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model           = glm::rotate(model, glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        model           = glm::rotate(model, glm::radians(m_roll), glm::vec3(0.0f, 0.0f, 1.0f));
        model           = glm::scale(model, m_scale);
        return model;
    }

    bool SceneObject::visible() const {
        return m_visible;
    }

    void SceneObject::set_visible(bool visible) {
        m_visible = visible;
    }

    const std::string &SceneObject::shader_name() const {
        return m_shader_name;
    }

    glm::vec3 SceneObject::emissive_color() const {
        return m_emissive_color;
    }

    void SceneObject::set_emissive_color(glm::vec3 emissive_color) {
        m_emissive_color = emissive_color;
    }

    float SceneObject::specular_strength() const {
        return m_specular_strength;
    }
} // namespace app
