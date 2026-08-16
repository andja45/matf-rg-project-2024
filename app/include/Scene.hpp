#ifndef SCENE_HPP
#define SCENE_HPP
#include <SceneObject.hpp>
#include <engine/graphics/Light.hpp>
#include <memory>
#include <vector>

namespace app {
class Scene {
    std::vector<std::unique_ptr<engine::graphics::Light>> m_lights;
    std::vector<SceneObject> m_objects;

public:
    void initialize();

    void add_light(std::unique_ptr<engine::graphics::Light> light);
    const std::vector<std::unique_ptr<engine::graphics::Light>> &lights() const;

    void add_object(SceneObject object);
    const std::vector<SceneObject> &objects() const;
};
}// namespace app
#endif//SCENE_HPP
