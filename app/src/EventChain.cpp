#include <EventChain.hpp>
#include <glm/common.hpp>

namespace app {
EventChain::EventChain(engine::graphics::Light *directional_light, engine::graphics::Light *point_light)
    : m_directional_light(directional_light)
    , m_point_light(point_light) {
}

void EventChain::enter_phase(EventChainPhase phase, float duration) {
    m_phase = phase;
    m_phase_elapsed = 0.0f;
    m_phase_duration = duration;
}

void EventChain::start() {
    if (m_phase != EventChainPhase::Idle) {
        return;
    }
    enter_phase(EventChainPhase::Dusk, DUSK_DURATION);
}

void EventChain::update(float dt) {
    if (m_phase == EventChainPhase::Idle || m_phase == EventChainPhase::CoreGlow) {
        return;
    }

    m_phase_elapsed += dt;
    float progress = glm::clamp(m_phase_elapsed / m_phase_duration, 0.0f, 1.0f);

    switch (m_phase) {
        case EventChainPhase::Dusk:
            m_directional_light->set_intensity(glm::mix(DEFAULT_INTENSITY, DIRECTIONAL_LIGHT_DIM_INTENSITY, progress));
            if (m_phase_elapsed >= m_phase_duration) {
                enter_phase(EventChainPhase::Night, NIGHT_DURATION);
            }
            break;
        case EventChainPhase::Night:
            m_point_light->set_intensity(glm::mix(DEFAULT_INTENSITY, POINT_LIGHT_GLOW_INTENSITY, progress));
            if (m_phase_elapsed >= m_phase_duration) {
                m_phase = EventChainPhase::CoreGlow;
            }
            break;
        default:
            break;
    }
}
}// namespace app
