#include <GUIController.hpp>
#include <SceneController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/DirectionalLight.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/PointLight.hpp>

namespace app {
    void SceneController::initialize() {
        engine::graphics::OpenGL::enable_depth_testing();

        auto directional_light = std::make_unique<engine::graphics::DirectionalLight>(
            glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f), glm::vec3(0.8f), glm::vec3(0.5f));
        engine::graphics::Light *directional_light_ptr = directional_light.get();
        m_scene.add_light(std::move(directional_light));

        auto point_light = std::make_unique<engine::graphics::PointLight>(
            glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.09f, 0.032f, glm::vec3(0.05f),
            glm::vec3(1.0f, 0.5f, 0.2f), glm::vec3(1.0f));
        engine::graphics::Light *point_light_ptr = point_light.get();
        m_scene.add_light(std::move(point_light));

        m_event_chain = std::make_unique<EventChain>(directional_light_ptr, point_light_ptr);
    }

    bool SceneController::loop() {
        const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).state() == engine::platform::Key::State::JustPressed) {
            return false;
        }
        return true;
    }

    void SceneController::poll_events() {
        const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_F1).state() == engine::platform::Key::State::JustPressed) {
            m_cursor_enabled = !m_cursor_enabled;
            platform->set_enable_cursor(m_cursor_enabled);
        }
        if (platform->key(engine::platform::KEY_SPACE).state() == engine::platform::Key::State::JustPressed) {
            m_event_chain->start();
        }
    }

    void SceneController::update() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        update_camera();
        m_event_chain->update(platform->dt());
    }

    void SceneController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void SceneController::draw() {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto shader   = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("blinn_phong");
        auto wreck    = engine::core::Controller::get<engine::resources::ResourcesController>()->
                model("wasteland_wagon");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_mat4("model", glm::scale(glm::mat4(1.0f), glm::vec3(0.005f)));
        shader->set_vec3("viewPos", graphics->camera()->Position);

        int point_light_index = 0;
        for (const auto &light: m_scene.lights()) {
            if (dynamic_cast<engine::graphics::PointLight *>(light.get()) != nullptr) {
                light->set_uniforms(shader, point_light_index);
                ++point_light_index;
            } else {
                light->set_uniforms(shader, 0);
            }
        }

        wreck->draw(shader);
        draw_skybox();
    }

    void SceneController::end_draw() {
        engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
    }

    void SceneController::draw_skybox() {
        auto shader      = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("skybox");
        auto skybox_cube = engine::core::Controller::get<engine::resources::ResourcesController>()->skybox(
            "desert_dusk");
        engine::core::Controller::get<engine::graphics::GraphicsController>()->draw_skybox(shader, skybox_cube);
    }

    void SceneController::update_camera() {
        auto gui = engine::core::Controller::get<GUIController>();

        if (gui->is_enabled()) {
            return;
        }

        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto camera   = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        float dt      = platform->dt();

        if (platform->key(engine::platform::KEY_W).state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
        }
        if (platform->key(engine::platform::KEY_S).state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
        }
        if (platform->key(engine::platform::KEY_A).state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
        }
        if (platform->key(engine::platform::KEY_D).state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
        }

        auto mouse = platform->mouse();
        camera->rotate_camera(mouse.dx, mouse.dy);
        camera->zoom(mouse.scroll);
    }
} // namespace app
