#version 440
layout(location = 0) in vec4 position;
layout(location = 0) out vec2 v_position;

void main()
{
	v_position = vec2(position.x * 0.5 + 0.5, position.y);
	gl_Position = position;
}
