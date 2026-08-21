#ifndef SCENECONTROLLER_HPP
#define SCENECONTROLLER_HPP
#include <EventChain.hpp>
#include <Scene.hpp>
#include <engine/core/Engine.hpp>
#include <engine/graphics/BloomEffect.hpp>
#include <engine/graphics/PointShadowFramebuffer.hpp>

namespace app {
class SceneController final : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "app::SceneController";
    }

    float directional_light_intensity() const;

    void set_directional_light_intensity(float intensity);

    bool directional_light_adjustable() const;

    bool bloom_enabled() const;

    void set_bloom_enabled(bool enabled);

    bool point_light_marker_enabled() const;

    void set_point_light_marker_enabled(bool enabled);

    bool point_shadows_enabled() const;

    void set_point_shadows_enabled(bool enabled);

    std::vector<SceneObject> &scene_objects();

    glm::vec3 point_light_position() const;

    void set_point_light_position(glm::vec3 position);

    glm::vec3 crystal_emissive_color() const;

    void set_crystal_emissive_color(glm::vec3 color);

    float bloom_threshold() const;

    void set_bloom_threshold(float threshold);

private:
    Scene m_scene;
    std::unique_ptr<EventChain> m_event_chain;
    std::unique_ptr<engine::graphics::BloomEffect> m_bloom;
    bool m_bloom_enabled{true};
    std::unique_ptr<engine::graphics::PointShadowFramebuffer> m_point_shadow_fb;
    bool m_point_shadows_enabled{true};

    float m_bloom_threshold{0.65f};

    static constexpr int POINT_SHADOW_SIZE = 1024;
    static constexpr float POINT_SHADOW_NEAR_PLANE = 0.05f;
    static constexpr float POINT_SHADOW_FAR_PLANE = 25.0f;

    void initialize() override;

    bool loop() override;

    void poll_events() override;

    void update() override;

    void begin_draw() override;

    void draw() override;

    void end_draw() override;

    void draw_skybox();

    void render_point_shadow_depth();

    void set_light_uniforms(engine::resources::Shader *shader);

    void update_camera();

    bool m_draw_gui{false};
    bool m_cursor_enabled{false};
};
}// namespace app
#endif//SCENECONTROLLER_HPP
