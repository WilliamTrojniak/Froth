#version 450

// layout(binding = 0) uniform UniformBufferObject {
//    mat4 model;
//    mat4 view;
//    mat4 proj;
// } ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNorm;
layout(location = 3) in vec2 inTexCoord;

layout(push_constant) uniform constants {
    mat4 mvp;
} pcs;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNorm;
layout(location = 2) out vec2 fragTexCoord;

void main() {
    gl_Position = pcs.mvp * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragNorm = inNorm;
    fragTexCoord = inTexCoord;
}
