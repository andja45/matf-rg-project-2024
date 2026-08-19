#ifndef SCENECONTROLLER_HPP
#define SCENECONTROLLER_HPP
#include <EventChain.hpp>
#include <Scene.hpp>
#include <engine/graphics/BloomEffect.hpp>

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

    private:
        Scene m_scene;
        std::unique_ptr<EventChain> m_event_chain;
        std::unique_ptr<engine::graphics::BloomEffect> m_bloom;
        bool m_bloom_enabled{true};

        void initialize() override;

        bool loop() override;

        void poll_events() override;

        void update() override;

        void begin_draw() override;

        void draw() override;

        void end_draw() override;

        void draw_skybox();

        void set_light_uniforms(engine::resources::Shader *shader);

        void update_camera();

        bool m_draw_gui{false};
        bool m_cursor_enabled{false};
    };
} // namespace app
#endif//SCENECONTROLLER_HPP
