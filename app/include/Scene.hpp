#ifndef SCENE_HPP
#define SCENE_HPP
#include <SceneObject.hpp>
#include <engine/graphics/Light.hpp>
#include <memory>
#include <vector>

namespace app {
    class Scene {
        std::vector<std::unique_ptr<engine::graphics::Light> > m_lights;
        std::vector<SceneObject> m_objects;

    public:
        void initialize();

        void add_light(std::unique_ptr<engine::graphics::Light> light);

        const std::vector<std::unique_ptr<engine::graphics::Light> > &lights() const;

        void add_object(SceneObject object);

        const std::vector<SceneObject> &objects() const;

        engine::graphics::Light *directional_light() const;

        engine::graphics::Light *point_light() const;

        SceneObject *planet();

        const SceneObject *planet() const;

        SceneObject *crystal();

        const SceneObject *crystal() const;

        SceneObject *light_marker();

        const SceneObject *light_marker() const;
    };
} // namespace app
#endif//SCENE_HPP
