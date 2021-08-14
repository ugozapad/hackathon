#version 330 core

out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D u_texture;

void main()
{
	//float gamma = 2.2;
	//vec3 diffuseColor = pow(texture(u_texture, texCoord).rgb, vec3(gamma));
    //fragColor = vec4(diffuseColor, 1.0);
    fragColor = vec4(texture(u_texture, texCoord).rgb, 1.0);
}