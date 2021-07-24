#version 330 core

out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D u_texture;

void main()
{
	vec3  s = texture(u_texture, texCoord).rgb;
	float r = s.r * s.g * s.b;  

	fragColor = vec4(s.r * s.g * s.b, s.r * s.g * s.b, s.r * s.g * s.b, 1.0);

	//fragColor = vec4(vec3(1.0 - texture(u_texture, texCoord)), 1.0);
    //fragColor = vec4(texture(u_texture, texCoord).rgb, 1.0);
    //fragColor = vec4(1.0, 0.0, 0.0, 0.0);
}