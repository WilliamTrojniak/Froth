#version 450

layout(binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main() {
  // outColor = vec4(texCoord, 0.0, 1.0);
    outColor = texture(texSampler, texCoord);
    // outColor = vec4(fragColor, 1);
}
