#version 330 core

out vec4 fragColor;

in vec2 attr_texcoord0;

uniform sampler2D u_texture;

void main()
{
	vec3 color = texture(u_texture, attr_texcoord0).rgb;
	
	fragColor = vec4(color, 1.0);
}