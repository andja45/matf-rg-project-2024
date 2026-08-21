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

    constexpr float GROUND_Y = -0.393f;

    SceneObject crystal("crystals_of_power", glm::vec3(1.1f, -0.4f, -6.9f), glm::vec3(0.011f),
                        glm::vec3(1.0f, 0.0f, 0.0f), -92.0f,
                        0.5f, "blinn_phong", glm::vec3(0.7608, 0.2431, 0.3961) * 1.55f);
    glm::vec3 crystal_position = crystal.position();
    add_object(std::move(crystal));

    glm::vec3 crystal_base_position = crystal_position + glm::vec3(-0.36f, 0.753f, 0.0f);

    glm::vec3 point_light_start_position = glm::vec3(crystal_base_position.x, -0.362f, crystal_base_position.z);

    auto point_light = std::make_unique<engine::graphics::PointLight>(
            point_light_start_position, 1.0f, 0.045f, 0.0075f, glm::vec3(0.05f),
            glm::vec3(0.7608, 0.2431, 0.3961) * 0.82f, glm::vec3(1.0f));
    add_light(std::move(point_light));

    add_object(SceneObject("wasteland_wagon", glm::vec3(1.75f, GROUND_Y + 0.038f, -8.0f), glm::vec3(0.005f),
                           glm::vec3(1.0f, 0.0f, 0.0f), -90.0f, 0.145f));

    add_object(SceneObject("planet_mars", glm::vec3(0.0f, -2.1f, -80.0f), glm::vec3(2.5f),
                           glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f, "planet"));

    add_object(SceneObject("rocky_landscape", glm::vec3(0.0f, -0.5f, -7.0f), glm::vec3(150.0f, 150.0f, 150.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.05f));

    add_object(SceneObject("rock_mh_15_hinge", glm::vec3(3.6f, -0.493f, -5.75f), glm::vec3(0.96f),
                           glm::vec3(0.0f, 1.0f, 0.0f), 243.0f, 0.15f));

    add_object(SceneObject("rock_mh_41_raft", glm::vec3(1.3f, -0.743f, -10.7f), glm::vec3(1.79f),
                           glm::vec3(1.0f, 0.0f, 0.0f), -3.0f, 0.1f));

    add_object(SceneObject("rock_mh_12_wall", glm::vec3(-2.6f, -0.443f, -5.5f), glm::vec3(1.511f),
                           glm::vec3(0.0f, 1.0f, 0.0f), 290.0f, 0.09f));

    add_object(SceneObject("rock_mh_39_arch", glm::vec3(-1.15f, -0.4f, -9.6f), glm::vec3(1.37f),
                           glm::vec3(0.0f, 1.0f, 0.0f), 48.0f, 0.065f));

    add_object(SceneObject("saguaro_cactus_02", glm::vec3(2.450f, -0.424f, -5.800f), glm::vec3(0.09f),
                           glm::vec3(1.0f, 0.0f, 0.0f), -90.0f, 0.15f));

    add_object(SceneObject("saguaro_cactus_02", glm::vec3(-2.850f, GROUND_Y, -10.400f), glm::vec3(0.08f),
                           glm::vec3(1.0f, 0.0f, 0.0f), -90.0f, 0.15f));

    add_object(SceneObject("saguaro_cactus_01", glm::vec3(1.450f, -0.496f, -9.000f), glm::vec3(0.013f),
                           glm::vec3(1.0f, 0.0f, 0.0f), 181.0f, 0.01f));

    add_object(SceneObject("saguaro_cactus_01", glm::vec3(0.950f, -0.496f, -15.650f), glm::vec3(0.011f),
                           glm::vec3(1.0f, 0.0f, 0.0f), 181.0f, 0.01f));

    SceneObject light_marker("light_marker_cube", point_light_start_position, glm::vec3(0.01f),
                             glm::vec3(0.5f, 1.0f, 0.0f), 0.0f, 0.0f, "light_marker",
                             glm::vec3(0.7608f, 0.2431f, 0.3961f) * 2.5f);
    add_object(std::move(light_marker));
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

std::vector<SceneObject> &Scene::objects_mutable() {
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

SceneObject *Scene::find_object_by_model_name(const std::string &model_name) {
    return const_cast<SceneObject *>(std::as_const(*this).find_object_by_model_name(model_name));
}

const SceneObject *Scene::find_object_by_model_name(const std::string &model_name) const {
    auto it = std::find_if(m_objects.begin(), m_objects.end(),
                           [&model_name](const SceneObject &object) {
                               return object.model_name() == model_name;
                           });
    return it == m_objects.end() ? nullptr : &(*it);
}

SceneObject *Scene::planet() {
    return find_object_by_model_name("planet_mars");
}

const SceneObject *Scene::planet() const {
    return find_object_by_model_name("planet_mars");
}

SceneObject *Scene::crystal() {
    return find_object_by_model_name("crystals_of_power");
}

const SceneObject *Scene::crystal() const {
    return find_object_by_model_name("crystals_of_power");
}

SceneObject *Scene::light_marker() {
    return find_object_by_model_name("light_marker_cube");
}

const SceneObject *Scene::light_marker() const {
    return find_object_by_model_name("light_marker_cube");
}
}// namespace app
