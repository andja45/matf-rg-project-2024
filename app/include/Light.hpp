#ifndef LIGHT_HPP
#define LIGHT_HPP
#include <glm/vec3.hpp>

namespace app {
struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float intensity = 1.0f;
};

struct PointLight {
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float intensity = 1.0f;
};
}// namespace app
#endif//LIGHT_HPP
