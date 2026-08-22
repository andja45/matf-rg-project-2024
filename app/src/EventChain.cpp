#include <EventChain.hpp>
#include <Scene.hpp>
#include <cmath>
#include <glm/common.hpp>

namespace app {
EventChain::EventChain(Scene *scene)
    : m_directional_light(scene->directional_light())
    , m_point_light(static_cast<engine::graphics::PointLight *>(scene->point_light()))
    , m_planet(scene->planet())
    , m_crystal(scene->crystal())
    , m_crystal_base_emissive(m_crystal->emissive_color()) {
    float wobble = 1.0f + 0.05f * std::sin(PLANET_HORIZON_ANGLE * 5.0f);
    glm::vec3 planet_position = m_planet->position();
    planet_position.x = PLANET_ORBIT_RADIUS_X * wobble * std::cos(PLANET_HORIZON_ANGLE);
    planet_position.y = PLANET_ORBIT_RADIUS_Y * wobble * std::sin(PLANET_HORIZON_ANGLE);
    m_planet->set_position(planet_position);
}

void EventChain::enter_phase(EventChainPhase phase, float duration) {
    m_phase = phase;
    m_phase_elapsed = 0.0f;
    m_phase_duration = duration;
}

void EventChain::apply_core_pulse() {
    float pulse = std::sin(m_elapsed_total * PULSE_SPEED) * PULSE_AMPLITUDE;
    float light_intensity = POINT_LIGHT_GLOW_INTENSITY + pulse * 0.25f;
    float emissive_intensity = POINT_LIGHT_GLOW_INTENSITY + pulse;
    m_point_light->set_intensity(light_intensity);
    m_crystal->set_emissive_color(m_crystal_base_emissive * emissive_intensity);
}

void EventChain::start() {
    if (m_phase != EventChainPhase::Idle) {
        return;
    }
    enter_phase(EventChainPhase::Dusk, DUSK_DURATION);
}

void EventChain::update(float dt) {
    if (m_phase == EventChainPhase::Idle) {
        return;
    }

    m_phase_elapsed += dt;
    m_elapsed_total += dt;

    switch (m_phase) {
        case EventChainPhase::Dusk: {
            float progress = glm::clamp(m_phase_elapsed / m_phase_duration, 0.0f, 1.0f);
            m_directional_light->set_intensity(glm::mix(DEFAULT_INTENSITY, DIRECTIONAL_LIGHT_DIM_INTENSITY, progress));
            if (m_phase_elapsed >= m_phase_duration) {
                enter_phase(EventChainPhase::Night, NIGHT_DURATION);
            }
            break;
        }
        case EventChainPhase::Night: {
            float progress = glm::clamp(m_phase_elapsed / m_phase_duration, 0.0f, 1.0f);
            float point_light_intensity = glm::mix(DEFAULT_INTENSITY, POINT_LIGHT_GLOW_INTENSITY, progress);
            m_point_light->set_intensity(point_light_intensity);
            m_crystal->set_emissive_color(m_crystal_base_emissive * point_light_intensity);

            m_skybox_darkness = glm::mix(SKYBOX_MIN_DARKNESS, SKYBOX_MAX_DARKNESS, progress);

            float planet_progress = glm::smoothstep(0.0f, 1.0f, progress);
            float angle = glm::mix(PLANET_HORIZON_ANGLE, PLANET_SKY_ANGLE, planet_progress);
            float wobble = 1.0f + 0.05f * std::sin(angle * 5.0f);
            glm::vec3 planet_position = m_planet->position();
            planet_position.x = PLANET_ORBIT_RADIUS_X * wobble * std::cos(angle);
            planet_position.y = PLANET_ORBIT_RADIUS_Y * wobble * std::sin(angle);
            m_planet->set_position(planet_position);

            if (m_phase_elapsed >= m_phase_duration) {
                m_elapsed_total = 0.0f;
                enter_phase(EventChainPhase::CoreEmerging, CORE_EMERGING_DURATION);
            }
            break;
        }
        case EventChainPhase::CoreEmerging: {
            apply_core_pulse();

            float progress = glm::clamp(m_phase_elapsed / m_phase_duration, 0.0f, 1.0f);
            float eased_progress = glm::smoothstep(0.0f, 1.0f, progress);
            glm::vec3 point_light_position = m_point_light->position();
            point_light_position.y = glm::mix(POINT_LIGHT_LOW_Y, POINT_LIGHT_HIGH_Y, eased_progress);
            m_point_light->set_position(point_light_position);

            if (m_phase_elapsed >= m_phase_duration) {
                enter_phase(EventChainPhase::CoreGlow, 0.0f);
            }
            break;
        }
        case EventChainPhase::CoreGlow: {
            apply_core_pulse();

            float bob = std::sin(m_phase_elapsed * POINT_LIGHT_BOB_SPEED) * POINT_LIGHT_BOB_AMPLITUDE;
            glm::vec3 point_light_position = m_point_light->position();
            point_light_position.y = POINT_LIGHT_HIGH_Y + bob;
            m_point_light->set_position(point_light_position);
            break;
        }
        default: break;
    }
}

bool EventChain::directional_light_locked() const {
    return m_phase == EventChainPhase::Dusk;
}

float EventChain::skybox_darkness() const {
    return m_skybox_darkness;
}
}// namespace app
