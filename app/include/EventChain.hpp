#ifndef EVENTCHAIN_HPP
#define EVENTCHAIN_HPP
#include <engine/graphics/Light.hpp>

namespace app {
    class Scene;
    class SceneObject;

    enum class EventChainPhase {
        Idle,
        Dusk,
        Night,
        CoreGlow
    };

    class EventChain {
        static constexpr float DUSK_DURATION                   = 3.0f;
        static constexpr float NIGHT_DURATION                  = 25.0f;
        static constexpr float DEFAULT_INTENSITY               = 1.0f;
        static constexpr float DIRECTIONAL_LIGHT_DIM_INTENSITY = 0.5f;
        static constexpr float POINT_LIGHT_GLOW_INTENSITY      = 2.5f;
        static constexpr float PULSE_SPEED                     = 2.0f;
        static constexpr float PULSE_AMPLITUDE                 = 0.8f;
        static constexpr float PLANET_HORIZON_ANGLE            = 3.49f;
        static constexpr float PLANET_SKY_ANGLE                = 1.57f;
        static constexpr float PLANET_ORBIT_RADIUS_X           = 38.0f;
        static constexpr float PLANET_ORBIT_RADIUS_Y           = 28.0f;
        static constexpr float SKYBOX_MIN_DARKNESS             = 0.0f;
        static constexpr float SKYBOX_MAX_DARKNESS             = 0.65f;

        EventChainPhase m_phase = EventChainPhase::Idle;
        float m_phase_elapsed   = 0.0f;
        float m_phase_duration  = 0.0f;

        engine::graphics::Light *m_directional_light;
        engine::graphics::Light *m_point_light;
        SceneObject *m_planet;
        SceneObject *m_crystal;
        glm::vec3 m_crystal_base_emissive;
        float m_skybox_darkness = 0.0f;

        void enter_phase(EventChainPhase phase, float duration);

    public:
        explicit EventChain(Scene *scene);

        void start();

        void update(float dt);

        bool directional_light_locked() const;

        float skybox_darkness() const;
    };
} // namespace app
#endif//EVENTCHAIN_HPP
