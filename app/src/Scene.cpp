#include <Scene.hpp>
#include <algorithm>
#include <engine/graphics/DirectionalLight.hpp>
#include <engine/graphics/PointLight.hpp>
#include <utility>

namespace app {
    void Scene::initialize() {
        auto directional_light = std::make_unique<engine::graphics::DirectionalLight>(
            glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.7, 0.3, 0.05) * 0.2f, glm::vec3(0.7, 0.3, 0.05) * 0.8f,
            glm::vec3(0.7, 0.3, 0.05) * 0.01f);
        add_light(std::move(directional_light));

        SceneObject crystal("crystals_of_power", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.01f),
                            glm::vec3(1.0f, 0.0f, 0.0f), -90.0f,
                            0.0f, "blinn_phong", glm::vec3(0.8902, 0.1333, 0.3608) * 1.0f);
        glm::vec3 crystal_position = crystal.position();
        add_object(std::move(crystal));

        // found with
        glm::vec3 crystal_base_position = crystal_position + glm::vec3(-0.364f, 0.061f, 0.0f);

        auto point_light = std::make_unique<engine::graphics::PointLight>(
            crystal_base_position, 1.0f, 0.045f, 0.0075f, glm::vec3(0.05f),
            glm::vec3(0.8902, 0.1333, 0.3608) * 1.4f, glm::vec3(1.0f));
        add_light(std::move(point_light));

        add_object(SceneObject("wasteland_wagon", glm::vec3(0.0f), glm::vec3(0.005f), glm::vec3(1.0f, 0.0f, 0.0f),
                               -90.0f, 0.1f));

        add_object(SceneObject("planet_mars", glm::vec3(0.0f, -2.1f, -80.0f), glm::vec3(2.6f),
                               glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f, "planet"));

        add_object(SceneObject("rocky_landscape", glm::vec3(0.0f, -0.5f, -7.0f), glm::vec3(150.0f, 150.0f, 150.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.05f));

        SceneObject light_marker("light_marker_cube", crystal_base_position, glm::vec3(0.05f),
                                 glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, "light_marker",
                                 glm::vec3(1.0f, 0.9f, 0.6f));
        light_marker.set_visible(false);
        add_object(std::move(light_marker));
    }

    void Scene::add_light(std::unique_ptr<engine::graphics::Light> light) {
        m_lights.push_back(std::move(light));
    }

    const std::vector<std::unique_ptr<engine::graphics::Light> > &Scene::lights() const {
        return m_lights;
    }

    void Scene::add_object(SceneObject object) {
        m_objects.push_back(std::move(object));
    }

    const std::vector<SceneObject> &Scene::objects() const {
        return m_objects;
    }

    engine::graphics::Light *Scene::directional_light() const {
        for (const auto &light: m_lights) {
            if (dynamic_cast<engine::graphics::DirectionalLight *>(light.get()) != nullptr) {
                return light.get();
            }
        }
        return nullptr;
    }

    engine::graphics::Light *Scene::point_light() const {
        for (const auto &light: m_lights) {
            if (dynamic_cast<engine::graphics::PointLight *>(light.get()) != nullptr) {
                return light.get();
            }
        }
        return nullptr;
    }

    SceneObject *Scene::planet() {
        auto it = std::find_if(m_objects.begin(), m_objects.end(),
                               [](const SceneObject &object) {
                                   return object.model_name() == "planet_mars";
                               });
        return it == m_objects.end() ? nullptr : &(*it);
    }

    const SceneObject *Scene::planet() const {
        auto it = std::find_if(m_objects.begin(), m_objects.end(),
                               [](const SceneObject &object) {
                                   return object.model_name() == "planet_mars";
                               });
        return it == m_objects.end() ? nullptr : &(*it);
    }

    SceneObject *Scene::crystal() {
        auto it = std::find_if(m_objects.begin(), m_objects.end(),
                               [](const SceneObject &object) {
                                   return object.model_name() == "crystals_of_power";
                               });
        return it == m_objects.end() ? nullptr : &(*it);
    }

    const SceneObject *Scene::crystal() const {
        auto it = std::find_if(m_objects.begin(), m_objects.end(),
                               [](const SceneObject &object) {
                                   return object.model_name() == "crystals_of_power";
                               });
        return it == m_objects.end() ? nullptr : &(*it);
    }

    SceneObject *Scene::light_marker() {
        auto it = std::find_if(m_objects.begin(), m_objects.end(),
                               [](const SceneObject &object) {
                                   return object.model_name() == "light_marker_cube";
                               });
        return it == m_objects.end() ? nullptr : &(*it);
    }

    const SceneObject *Scene::light_marker() const {
        auto it = std::find_if(m_objects.begin(), m_objects.end(),
                               [](const SceneObject &object) {
                                   return object.model_name() == "light_marker_cube";
                               });
        return it == m_objects.end() ? nullptr : &(*it);
    }
} // namespace app
