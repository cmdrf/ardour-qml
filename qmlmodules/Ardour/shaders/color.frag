#version 440
layout(location = 0) in vec2 v_position;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D minTex;
layout(binding = 2) uniform sampler2D maxTex;

void main()
{
	float min = texture(minTex, vec2(0, v_position.x)).r;
	float max = texture(maxTex, vec2(0, v_position.x)).r;

	fragColor = vec4(min, max, 0.0, 1.0);
}
