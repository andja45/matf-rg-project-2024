#ifndef SCENE_HPP
#define SCENE_HPP
#include <engine/graphics/Light.hpp>
#include <memory>
#include <vector>

namespace app {
class Scene {
    std::vector<std::unique_ptr<engine::graphics::Light>> m_lights;

public:
    void add_light(std::unique_ptr<engine::graphics::Light> light);

    const std::vector<std::unique_ptr<engine::graphics::Light>> &lights() const;
};
}// namespace app
#endif//SCENE_HPP
