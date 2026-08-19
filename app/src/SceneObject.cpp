#include <SceneObject.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace app {
    SceneObject::SceneObject(std::string model_name, glm::vec3 position, glm::vec3 scale,
                             glm::vec3 rotation_axis, float rotation_angle,
                             float specular_strength,
                             std::string shader_name, glm::vec3 emissive_color)
    : m_model_name(std::move(model_name))
  , m_position(position)
  , m_rotation_axis(rotation_axis)
  , m_rotation_angle(rotation_angle)
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

    glm::vec3 SceneObject::rotation_axis() const {
        return m_rotation_axis;
    }

    void SceneObject::set_rotation_axis(glm::vec3 axis) {
        m_rotation_axis = axis;
    }

    float SceneObject::rotation_angle() const {
        return m_rotation_angle;
    }

    void SceneObject::set_rotation_angle(float angle_degrees) {
        m_rotation_angle = angle_degrees;
    }

    glm::mat4 SceneObject::model_matrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, m_position);
        model           = glm::rotate(model, glm::radians(m_rotation_angle), m_rotation_axis);
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
