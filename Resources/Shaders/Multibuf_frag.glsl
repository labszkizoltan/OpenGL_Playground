
#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

in vec3 outColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(outColor, 1.0f);
	// FragColor = vec4(1, 1, 1, 1);
}
