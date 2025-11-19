#version 440
layout(location = 0) in vec2 v_position;
layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = vec4(v_position, 0.0, 1.0);
}
