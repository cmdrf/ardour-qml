#version 440
layout(location = 0) in vec2 v_position;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D minTex;
layout(binding = 2) uniform sampler2D maxTex;

float coefficient_to_dB (float coeff)
{
	return 20.0f * log(coeff) / log(10);
}

float _log_meter (float power, float lower_db, float upper_db, float non_linearity)
{
	return (power < lower_db ? 0.0 : pow((power-lower_db)/(upper_db-lower_db), non_linearity));
}

float alt_log_meter (float power)
{
	return _log_meter (power, -192.0, 0.0, 8.0);
}

void main()
{
	float min = texture(minTex, vec2(0, v_position.x)).r;
	float max = texture(maxTex, vec2(0, v_position.x)).r;

	float pmax = 1.0, pmin = -1.0;

	pmax = sign(max) * alt_log_meter (coefficient_to_dB (abs(max)));
	pmin = sign(min) * alt_log_meter (coefficient_to_dB (abs(min)));

	if(abs(v_position.y) < 0.01)
		fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else if(v_position.y > pmin && v_position.y < pmax)
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	else
		fragColor = vec4(0.4, 0.7, 0.8, 1.0);
}
