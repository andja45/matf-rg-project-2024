#include <Application.hpp>
#include <GUIController.hpp>
#include <SceneController.hpp>

namespace app {
void Application::app_setup() {
    auto scene_controller = register_controller<SceneController>();
    auto gui_controller = register_controller<GUIController>();
    scene_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    gui_controller->after(scene_controller);
}
}// namespace app
