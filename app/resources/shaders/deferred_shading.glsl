//#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
}

//#shader fragment
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gEmissive;

uniform vec3 viewPos;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 1
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform samplerCube pointShadowMap;
uniform float pointShadowFarPlane;
uniform bool pointShadows;

vec3 gridSamplingDisk[20] = vec3[](
vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float hash12(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * 0.1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

float PointShadowCalculation(vec3 fragPos, vec3 lightPos) {
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / pointShadowFarPlane)) / 25.0;

    float theta = hash12(gl_FragCoord.xy) * 6.2831853;
    vec3 axis = normalize(fragToLight);
    float c = cos(theta);
    float s = sin(theta);

    for (int i = 0; i < samples; ++i) {
        vec3 offset = gridSamplingDisk[i];
        vec3 rotatedOffset = offset * c + cross(axis, offset) * s + axis * dot(axis, offset) * (1.0 - c);

        float closestDepth = texture(pointShadowMap, fragToLight + rotatedOffset * diskRadius).r;
        closestDepth *= pointShadowFarPlane;
        if (currentDepth - bias > closestDepth) {
            shadow += 1.0;
        }
    }
    return shadow / float(samples);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 albedo, float specularStrength, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 albedo, float specularStrength, vec3 viewDir);

void main() {
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec4 normal_sample = texture(gNormal, TexCoords);
    vec3 normal = normalize(normal_sample.rgb);
    float specularStrength = normal_sample.a;
    vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
    vec3 emissive = texture(gEmissive, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, normal, albedo, specularStrength, viewDir);
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], normal, FragPos, albedo, specularStrength, viewDir);
    }
    result += emissive;

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 albedo, float specularStrength, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo;
    vec3 specular = light.specular * spec * specularStrength;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 albedo, float specularStrength, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo;
    vec3 specular = light.specular * spec * specularStrength;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = pointShadows ? PointShadowCalculation(fragPos, light.position) : 0.0;
    return ambient + (1.0 - shadow) * (diffuse + specular);
}
