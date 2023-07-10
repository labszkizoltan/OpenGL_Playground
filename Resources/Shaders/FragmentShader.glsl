
#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

in vec3 outColor;
out vec4 FragColor;

void main()
{
//	vec4 texColor = texture(u_Texture, v_TexCoord);
	FragColor = vec4(outColor, 1.0f);
//	FragColor = texColor * vec4(outColor, 1.0f);

}
