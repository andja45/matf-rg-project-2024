#ifndef EVENTCHAIN_HPP
#define EVENTCHAIN_HPP
#include <engine/graphics/Light.hpp>

namespace app {
enum class EventChainPhase { Idle,
                             Dusk,
                             Night,
                             CoreGlow };

class EventChain {
    static constexpr float DUSK_DURATION = 3.0f;
    static constexpr float NIGHT_DURATION = 4.0f;
    static constexpr float DEFAULT_INTENSITY = 1.0f;
    static constexpr float DIRECTIONAL_LIGHT_DIM_INTENSITY = 0.1f;
    static constexpr float POINT_LIGHT_GLOW_INTENSITY = 3.0f;

    EventChainPhase m_phase = EventChainPhase::Idle;
    float m_phase_elapsed = 0.0f;
    float m_phase_duration = 0.0f;

    engine::graphics::Light *m_directional_light;
    engine::graphics::Light *m_point_light;

    void enter_phase(EventChainPhase phase, float duration);

public:
    EventChain(engine::graphics::Light *directional_light, engine::graphics::Light *point_light);

    void start();

    void update(float dt);
};
}// namespace app
#endif//EVENTCHAIN_HPP
