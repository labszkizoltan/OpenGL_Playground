
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 texCoord;

uniform float angle;

out vec3 outColor;
out vec2 v_TexCoord;

void main()
{
	float x = cos(angle)*aPos.x + sin(angle)*aPos.y;
	float y = -sin(angle)*aPos.x + cos(angle)*aPos.y;
	gl_Position = vec4(x, y, aPos.z, 1.0f);
	outColor = aColor;
	v_TexCoord = texCoord;
}
