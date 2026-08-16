#include <Scene.hpp>
#include <utility>

namespace app {
void Scene::add_light(std::unique_ptr<engine::graphics::Light> light) {
    m_lights.push_back(std::move(light));
}

const std::vector<std::unique_ptr<engine::graphics::Light>> &Scene::lights() const {
    return m_lights;
}
}// namespace app
