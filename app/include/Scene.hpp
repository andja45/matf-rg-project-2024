#ifndef SCENE_HPP
#define SCENE_HPP
#include <Light.hpp>
#include <SceneObject.hpp>
#include <string_view>
#include <vector>

namespace app {
class Scene {
    DirectionalLight m_directional_light;
    PointLight m_point_light;
    std::vector<SceneObject> m_objects;

public:
    void initialize();

    void add_object(SceneObject object);

    const std::vector<SceneObject> &objects() const;

    std::vector<SceneObject> &objects_mutable();

    DirectionalLight &directional_light();

    const DirectionalLight &directional_light() const;

    PointLight &point_light();

    const PointLight &point_light() const;

    SceneObject *find_object_by_model_name(std::string_view model_name);

    const SceneObject *find_object_by_model_name(std::string_view model_name) const;
};
}// namespace app
#endif//SCENE_HPP
