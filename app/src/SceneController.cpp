#include <RendererFactory.hpp>
#include <SceneController.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <glm/trigonometric.hpp>

namespace app {
void SceneController::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
    m_scene.initialize();
    m_event_chain = std::make_unique<EventChain>(&m_scene);

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->set_enable_cursor(m_cursor_enabled);

    m_bloom = std::make_unique<engine::graphics::BloomEffect>(platform->window()->width(),
                                                              platform->window()->height());

    m_point_shadow_fb = std::make_unique<engine::graphics::PointShadowFramebuffer>(POINT_SHADOW_SIZE);

    m_renderer = RendererFactory::create(m_renderer_type);
    m_renderer->initialize(platform->window()->width(), platform->window()->height());
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
    float dt = platform->dt();
    update_camera();
    m_event_chain->update(dt);

    constexpr float PLANET_SPIN_SPEED = 1.5f;
    auto planet = m_scene.find_object_by_model_name("planet_mars");
    planet->set_rotation_angle(planet->rotation_angle() + dt * PLANET_SPIN_SPEED);

    auto point_light_position = m_scene.point_light().position;
    auto light_marker = m_scene.find_object_by_model_name("light_marker_cube");
    light_marker->set_position(point_light_position);
    light_marker->set_rotation_angle(glm::degrees(platform->frame_time().current));
}

void SceneController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void SceneController::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    render_point_shadow_depth();
    m_bloom->begin_scene_capture();

    std::vector<SceneObject *> lit_objects;
    for (auto &object: m_scene.objects_mutable()) {
        if (object.visible() && object.shader_name() == "lighting") {
            lit_objects.push_back(&object);
        }
    }
    m_renderer->render(lit_objects, m_scene.directional_light(), m_scene.point_light(),
                       graphics->projection_matrix(), graphics->camera()->view_matrix(), graphics->camera()->Position,
                       m_point_shadow_fb->depth_cubemap_id(), POINT_SHADOW_FAR_PLANE, m_point_shadows_enabled,
                       m_bloom->scene_fbo_id());

    for (const auto &object: m_scene.objects()) {
        if (!object.visible() || object.shader_name() == "lighting") {
            continue;
        }

        auto shader = resources->shader(object.shader_name());
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_float("time", platform->frame_time().current);
        shader->set_vec3("emissiveColor", object.emissive_color());
        shader->set_float("specularStrength", object.specular_strength());

        shader->set_mat4("model", object.model_matrix());
        resources->model(object.model_name())->draw(shader);
    }

    draw_skybox();

    m_bloom->apply(resources->shader("bloom_extract"), resources->shader("bloom_blur"),
                   resources->shader("bloom_combine"), m_bloom_enabled, m_bloom_threshold);
}

void SceneController::render_point_shadow_depth() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    auto light_position = m_scene.point_light().position;

    auto matrices = engine::graphics::PointShadowFramebuffer::calculate_shadow_matrices(
            light_position, POINT_SHADOW_NEAR_PLANE, POINT_SHADOW_FAR_PLANE, POINT_SHADOW_SIZE, POINT_SHADOW_SIZE);

    auto shader = resources->shader("point_shadow_depth");
    shader->use();
    for (int face = 0; face < 6; ++face) {
        shader->set_mat4("shadowMatrices[" + std::to_string(face) + "]", matrices[face]);
    }
    shader->set_vec3("lightPos", light_position);
    shader->set_float("far_plane", POINT_SHADOW_FAR_PLANE);

    m_point_shadow_fb->bind();
    for (const auto &object: m_scene.objects()) {
        if (!object.visible()) {
            continue;
        }
        shader->set_mat4("model", object.model_matrix());
        resources->model(object.model_name())->draw(shader);
    }
    m_point_shadow_fb->unbind(platform->window()->width(), platform->window()->height());
}

void SceneController::end_draw() {
    engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
}

void SceneController::terminate() {
    m_bloom->destroy();
    m_point_shadow_fb->destroy();
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
    return m_scene.directional_light().intensity;
}

void SceneController::set_directional_light_intensity(float intensity) {
    if (m_event_chain->directional_light_locked()) {
        return;
    }
    m_scene.directional_light().intensity = intensity;
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
    return m_scene.find_object_by_model_name("light_marker_cube")->visible();
}

void SceneController::set_point_light_marker_enabled(bool enabled) {
    m_scene.find_object_by_model_name("light_marker_cube")->set_visible(enabled);
}

bool SceneController::point_shadows_enabled() const {
    return m_point_shadows_enabled;
}

void SceneController::set_point_shadows_enabled(bool enabled) {
    m_point_shadows_enabled = enabled;
}

std::vector<SceneObject> &SceneController::scene_objects() {
    return m_scene.objects_mutable();
}

glm::vec3 SceneController::point_light_position() const {
    return m_scene.point_light().position;
}

void SceneController::set_point_light_position(glm::vec3 position) {
    m_scene.point_light().position = position;
}

float SceneController::bloom_threshold() const {
    return m_bloom_threshold;
}

void SceneController::set_bloom_threshold(float threshold) {
    m_bloom_threshold = threshold;
}

RendererType SceneController::renderer_type() const {
    return m_renderer_type;
}

void SceneController::set_renderer_type(RendererType type) {
    if (type == m_renderer_type) {
        return;
    }
    m_renderer_type = type;
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    m_renderer = RendererFactory::create(m_renderer_type);
    m_renderer->initialize(platform->window()->width(), platform->window()->height());
}

void SceneController::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    float dt = platform->dt();

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

    if (!m_cursor_enabled) {
        auto mouse = platform->mouse();
        camera->rotate_camera(mouse.dx, mouse.dy);
        camera->zoom(mouse.scroll);
    }
}
}// namespace app
