#include <Scene.hpp>
#include <engine/graphics/DirectionalLight.hpp>
#include <engine/graphics/PointLight.hpp>
#include <utility>

namespace app {
void Scene::initialize() {
    auto directional_light = std::make_unique<engine::graphics::DirectionalLight>(
            glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f), glm::vec3(0.8f), glm::vec3(0.5f));
    add_light(std::move(directional_light));

    SceneObject crystal("crystals_of_power", glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.005f), 0.0f, -90.0f, 0.0f);
    glm::vec3 crystal_position = crystal.position();
    add_object(std::move(crystal));

    auto point_light = std::make_unique<engine::graphics::PointLight>(
            crystal_position, 1.0f, 0.09f, 0.032f, glm::vec3(0.05f), glm::vec3(1.0f, 0.5f, 0.2f), glm::vec3(1.0f));
    add_light(std::move(point_light));

    add_object(SceneObject("wasteland_wagon", glm::vec3(0.0f), glm::vec3(0.005f), 0.0f, -90.0f, 0.0f));
}

void Scene::add_light(std::unique_ptr<engine::graphics::Light> light) {
    m_lights.push_back(std::move(light));
}

const std::vector<std::unique_ptr<engine::graphics::Light>> &Scene::lights() const {
    return m_lights;
}

void Scene::add_object(SceneObject object) {
    m_objects.push_back(std::move(object));
}

const std::vector<SceneObject> &Scene::objects() const {
    return m_objects;
}
}// namespace app
