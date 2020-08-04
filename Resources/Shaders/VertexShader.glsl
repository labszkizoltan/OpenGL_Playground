
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aID;

uniform vec3 translation[3];

out vec3 outColor;

void main()
{
//	vec3 translation[2] = vec3[](vec3(-0.2, 0.0, 0.2), vec3(0.2, 0.0, 0.0));

	int index = int(aID);
	gl_Position = vec4(aPos + translation[index], 1.0f);

	outColor = aColor;
}
