#ifndef SCENECONTROLLER_HPP
#define SCENECONTROLLER_HPP

namespace app {
    class SceneController final : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::SceneController";
        }

    private:
        void initialize() override;

        bool loop() override;

        void poll_events() override;

        void update() override;

        void begin_draw() override;

        void draw() override;

        void end_draw() override;

        void draw_skybox();

        void update_camera();

        bool m_draw_gui{false};
        bool m_cursor_enabled{true};
    };
} // namespace app
#endif//SCENECONTROLLER_HPP
