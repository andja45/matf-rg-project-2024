// clang-format off
#include <glad/glad.h>
// clang-format on
#include <DeferredRenderer.hpp>
#include <SceneObject.hpp>
#include <engine/core/Controller.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Model.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <engine/resources/Shader.hpp>

namespace app {
DeferredRenderer::~DeferredRenderer() {
    m_gbuffer->destroy();
    CHECKED_GL_CALL(glDeleteVertexArrays, 1, &m_quad_vao);
    CHECKED_GL_CALL(glDeleteBuffers, 1, &m_quad_vbo);
}

void DeferredRenderer::initialize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;
    m_gbuffer = std::make_unique<engine::graphics::GBuffer>(width, height);

    float quad_vertices[] = {
            -1.0f,
            1.0f,
            0.0f,
            1.0f,// top-left
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,// bottom-left
            1.0f,
            1.0f,
            1.0f,
            1.0f,// top-right
            1.0f,
            -1.0f,
            1.0f,
            0.0f,// bottom-right
    };
    CHECKED_GL_CALL(glGenVertexArrays, 1, &m_quad_vao);
    CHECKED_GL_CALL(glGenBuffers, 1, &m_quad_vbo);
    CHECKED_GL_CALL(glBindVertexArray, m_quad_vao);
    CHECKED_GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, m_quad_vbo);
    CHECKED_GL_CALL(glBufferData, GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
    CHECKED_GL_CALL(glEnableVertexAttribArray, 0);
    CHECKED_GL_CALL(glVertexAttribPointer, 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    CHECKED_GL_CALL(glEnableVertexAttribArray, 1);
    CHECKED_GL_CALL(glVertexAttribPointer, 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                    (void *) (2 * sizeof(float)));
    CHECKED_GL_CALL(glBindVertexArray, 0);
}

void DeferredRenderer::resize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;
    m_gbuffer->destroy();
    m_gbuffer = std::make_unique<engine::graphics::GBuffer>(width, height);
}

void DeferredRenderer::render(const std::vector<SceneObject *> &lit_objects,
                              const DirectionalLight &dir_light, const PointLight &point_light,
                              const glm::mat4 &projection, const glm::mat4 &view, glm::vec3 view_pos,
                              uint32_t point_shadow_cubemap, float point_shadow_far_plane,
                              bool point_shadows_enabled,
                              uint32_t target_fbo) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    // geometry pass
    m_gbuffer->bind();
    auto gbuffer_shader = resources->shader("g_buffer");
    gbuffer_shader->use();
    gbuffer_shader->set_mat4("projection", projection);
    gbuffer_shader->set_mat4("view", view);
    for (auto *object: lit_objects) {
        gbuffer_shader->set_mat4("model", object->model_matrix());
        gbuffer_shader->set_vec3("emissiveColor", object->emissive_color());
        gbuffer_shader->set_float("specularStrength", object->specular_strength());
        resources->model(object->model_name())->draw(gbuffer_shader);
    }

    // lighting pass
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, target_fbo);
    auto lighting_shader = resources->shader("deferred_shading");
    lighting_shader->use();
    lighting_shader->set_vec3("viewPos", view_pos);

    lighting_shader->set_vec3("dirLight.direction", dir_light.direction);
    lighting_shader->set_vec3("dirLight.ambient", dir_light.ambient * dir_light.intensity);
    lighting_shader->set_vec3("dirLight.diffuse", dir_light.diffuse * dir_light.intensity);
    lighting_shader->set_vec3("dirLight.specular", dir_light.specular * dir_light.intensity);

    lighting_shader->set_vec3("pointLights[0].position", point_light.position);
    lighting_shader->set_float("pointLights[0].constant", point_light.constant);
    lighting_shader->set_float("pointLights[0].linear", point_light.linear);
    lighting_shader->set_float("pointLights[0].quadratic", point_light.quadratic);
    lighting_shader->set_vec3("pointLights[0].ambient", point_light.ambient * point_light.intensity);
    lighting_shader->set_vec3("pointLights[0].diffuse", point_light.diffuse * point_light.intensity);
    lighting_shader->set_vec3("pointLights[0].specular", point_light.specular * point_light.intensity);

    lighting_shader->set_int("gPosition", 0);
    lighting_shader->set_int("gNormal", 1);
    lighting_shader->set_int("gAlbedoSpec", 2);
    lighting_shader->set_int("gEmissive", 3);
    lighting_shader->set_int("pointShadowMap", 4);
    lighting_shader->set_float("pointShadowFarPlane", point_shadow_far_plane);
    lighting_shader->set_bool("pointShadows", point_shadows_enabled);

    m_gbuffer->bind_textures(0, 1, 2, 3);
    engine::graphics::OpenGL::bind_texture_cube_to_unit(4, point_shadow_cubemap);

    CHECKED_GL_CALL(glBindVertexArray, m_quad_vao);
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLE_STRIP, 0, 4);
    CHECKED_GL_CALL(glBindVertexArray, 0);

    engine::graphics::OpenGL::blit_depth_buffer(m_gbuffer->fbo_id(), target_fbo, m_width, m_height);
}
}// namespace app
