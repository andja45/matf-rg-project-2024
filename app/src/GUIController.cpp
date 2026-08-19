#include <GUIController.hpp>
#include <SceneController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <imgui.h>

namespace app {
    void GUIController::initialize() {
        set_enable(false);
    }

    void GUIController::poll_events() {
        const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_F2).state() == engine::platform::Key::State::JustPressed) {
            set_enable(!is_enabled());
        }
    }

    void GUIController::draw() {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera   = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();

        graphics->begin_gui();

        ImGui::Begin("Camera info");
        const auto &c = *camera;
        ImGui::Text("Camera position: (%f, %f, %f)", c.Position.x, c.Position.y, c.Position.z);
        ImGui::Text("(Yaw, Pitch): (%f, %f)", c.Yaw, c.Pitch);
        ImGui::Text("Camera front: (%f, %f, %f)", c.Front.x, c.Front.y, c.Front.z);
        ImGui::End();

        auto scene_controller             = engine::core::Controller::get<SceneController>();
        float directional_light_intensity = scene_controller->directional_light_intensity();

        ImGui::Begin("Lighting");
        ImGui::BeginDisabled(!scene_controller->directional_light_adjustable());
        if (ImGui::SliderFloat("Directional light intensity", &directional_light_intensity, 0.0f, 2.0f)) {
            scene_controller->set_directional_light_intensity(directional_light_intensity);
        }
        ImGui::EndDisabled();

        bool bloom_enabled = scene_controller->bloom_enabled();
        if (ImGui::Checkbox("Bloom", &bloom_enabled)) {
            scene_controller->set_bloom_enabled(bloom_enabled);
        }

        bool point_light_marker_enabled = scene_controller->point_light_marker_enabled();
        if (ImGui::Checkbox("Point light marker", &point_light_marker_enabled)) {
            scene_controller->set_point_light_marker_enabled(point_light_marker_enabled);
        }

        ImGui::End();

        graphics->end_gui();
    }
} // namespace app
