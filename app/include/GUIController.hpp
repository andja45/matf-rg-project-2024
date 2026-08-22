#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP

#include <engine/core/Engine.hpp>
#include <glm/glm.hpp>
#include <vector>

namespace app {
class SceneController;

class GUIController final : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::GUIController";
    }

private:
    void initialize() override;

    void poll_events() override;

    void draw() override;

    void draw_scene_editor(SceneController *scene_controller);
};
}// namespace app
#endif//GUICONTROLLER_HPP
