#version 450

layout(binding = 0) uniform sampler2D texSampler[2];

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform constants {
    mat4 _;
    uint texIndex;
} pcs;

void main() {
    outColor = vec4(fragColor, 1.0) * texture(texSampler[pcs.texIndex], texCoord);
}
