#include <GUIController.hpp>
#include <SceneController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/PointLight.hpp>

namespace app {
    void SceneController::initialize() {
        engine::graphics::OpenGL::enable_depth_testing();
        m_scene.initialize();
        m_event_chain = std::make_unique<EventChain>(&m_scene);

        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->set_enable_cursor(m_cursor_enabled);

        m_bloom = std::make_unique<engine::graphics::BloomEffect>();
        m_bloom->init(platform->window()->width(), platform->window()->height());
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
        float dt      = platform->dt();
        update_camera();
        m_event_chain->update(dt);

        constexpr float PLANET_SPIN_SPEED = 1.5f;
        auto planet                       = m_scene.planet();
        planet->set_rotation_angle(planet->rotation_angle() + dt * PLANET_SPIN_SPEED);
    }

    void SceneController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void SceneController::draw() {
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto platform  = engine::core::Controller::get<engine::platform::PlatformController>();

        m_bloom->begin_scene_capture();

        for (const auto &object: m_scene.objects()) {
            if (!object.visible()) {
                continue;
            }

            auto shader = resources->shader(object.shader_name());
            shader->use();
            shader->set_mat4("projection", graphics->projection_matrix());
            shader->set_mat4("view", graphics->camera()->view_matrix());
            shader->set_vec3("viewPos", graphics->camera()->Position);
            shader->set_float("time", platform->frame_time().current);
            set_light_uniforms(shader);
            shader->set_vec3("emissiveColor", object.emissive_color());
            shader->set_float("specularStrength", object.specular_strength());

            shader->set_mat4("model", object.model_matrix());
            resources->model(object.model_name())->draw(shader);
        }

        draw_skybox();

        m_bloom->apply(resources->shader("bloom_extract"), resources->shader("bloom_blur"),
                       resources->shader("bloom_combine"), m_bloom_enabled);
    }

    void SceneController::set_light_uniforms(engine::resources::Shader *shader) {
        int point_light_index = 0;
        for (const auto &light: m_scene.lights()) {
            if (dynamic_cast<engine::graphics::PointLight *>(light.get()) != nullptr) {
                light->set_uniforms(shader, point_light_index);
                ++point_light_index;
            } else {
                light->set_uniforms(shader, 0);
            }
        }
    }

    void SceneController::end_draw() {
        engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
    }

    void SceneController::draw_skybox() {
        auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("skybox");
        shader->use();
        shader->set_float("darkness", m_event_chain->skybox_darkness());
        auto skybox_cube = engine::core::Controller::get<engine::resources::ResourcesController>()->skybox(
            "soft2");
        engine::core::Controller::get<engine::graphics::GraphicsController>()->draw_skybox(shader, skybox_cube);
    }

    float SceneController::directional_light_intensity() const {
        return m_scene.directional_light()->intensity();
    }

    void SceneController::set_directional_light_intensity(float intensity) {
        if (m_event_chain->directional_light_locked()) {
            return;
        }
        m_scene.directional_light()->set_intensity(intensity);
    }

    bool SceneController::directional_light_adjustable() const {
        return !m_event_chain->directional_light_locked();
    }

    bool SceneController::bloom_enabled() const {
        return m_bloom_enabled;
    }

    void SceneController::set_bloom_enabled(bool enabled) {
        m_bloom_enabled = enabled;
    }

    bool SceneController::point_light_marker_enabled() const {
        return m_scene.light_marker()->visible();
    }

    void SceneController::set_point_light_marker_enabled(bool enabled) {
        m_scene.light_marker()->set_visible(enabled);
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
