#include <GUIController.hpp>
#include <SceneController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <imgui.h>
#include <string>

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
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();

    graphics->begin_gui();

    ImGui::Begin("Camera info");
    const auto &c = *camera;
    ImGui::Text("Camera position: (%f, %f, %f)", c.Position.x, c.Position.y, c.Position.z);
    ImGui::Text("(Yaw, Pitch): (%f, %f)", c.Yaw, c.Pitch);
    ImGui::Text("Camera front: (%f, %f, %f)", c.Front.x, c.Front.y, c.Front.z);
    ImGui::End();

    auto scene_controller = engine::core::Controller::get<SceneController>();
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

    float bloom_threshold = scene_controller->bloom_threshold();
    if (ImGui::DragFloat("Bloom threshold", &bloom_threshold, 0.005f, 0.0f, 3.0f)) {
        scene_controller->set_bloom_threshold(bloom_threshold);
    }

    bool point_shadows_enabled = scene_controller->point_shadows_enabled();
    if (ImGui::Checkbox("Point shadows", &point_shadows_enabled)) {
        scene_controller->set_point_shadows_enabled(point_shadows_enabled);
    }

    ImGui::End();

    draw_scene_editor(scene_controller);

    graphics->end_gui();
}

void GUIController::draw_scene_editor(SceneController *scene_controller) {
    ImGui::Begin("Scene Editor");

    if (ImGui::CollapsingHeader("Point light crystal glow")) {
        glm::vec3 light_position = scene_controller->point_light_position();
        if (ImGui::DragFloat3("Light position", &light_position.x, 0.05f)) {
            scene_controller->set_point_light_position(light_position);
        }

        bool marker_enabled = scene_controller->point_light_marker_enabled();
        if (ImGui::Checkbox("Show light marker", &marker_enabled)) {
            scene_controller->set_point_light_marker_enabled(marker_enabled);
        }
    }

    auto &objects = scene_controller->scene_objects();
    for (size_t i = 0; i < objects.size(); ++i) {
        SceneObject &object = objects[i];
        std::string id = std::to_string(i);
        std::string header = object.model_name() + "##" + id;

        if (!ImGui::CollapsingHeader(header.c_str())) {
            continue;
        }

        glm::vec3 position = object.position();
        if (ImGui::DragFloat3(("Position##" + id).c_str(), &position.x, 0.05f)) {
            object.set_position(position);
        }

        float uniform_scale = object.scale().x;
        if (ImGui::DragFloat(("Scale##" + id).c_str(), &uniform_scale, 0.01f, 0.001f, 200.0f)) {
            object.set_scale(glm::vec3(uniform_scale));
        }

        float specular_strength = object.specular_strength();
        if (ImGui::DragFloat(("Specular strength##" + id).c_str(), &specular_strength, 0.005f, 0.0f, 5.0f)) {
            object.set_specular_strength(specular_strength);
        }

        bool visible = object.visible();
        if (ImGui::Checkbox(("Visible##" + id).c_str(), &visible)) {
            object.set_visible(visible);
        }
    }

    ImGui::End();
}
}// namespace app
