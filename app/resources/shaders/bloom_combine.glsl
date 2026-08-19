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

uniform sampler2D sceneTexture;
uniform sampler2D bloomTexture;
uniform bool bloom;

void main() {
    const float exposure = 1.0;
    vec3 hdr_color = texture(sceneTexture, TexCoords).rgb;
    vec3 bloom_color = texture(bloomTexture, TexCoords).rgb;
    if (bloom) {
        hdr_color += bloom_color;
    }

    vec3 result = vec3(1.0) - exp(-hdr_color * exposure); // tone mapping
    FragColor = vec4(result, 1.0);
}
