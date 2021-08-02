#version 330 core
layout (location = 0) out vec4 gbuffer_pos;
layout (location = 1) out vec3 gbuffer_normal;
layout (location = 2) out vec4 gbuffer_albedo_spec;

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_albedoTexture;
uniform bool u_selfillum;

void main()
{
    gbuffer_pos.rgb = Pos;
	
	if (u_selfillum)
		gbuffer_pos.a = 1.0;
	else
		gbuffer_pos.a = 0.0;
	
    gbuffer_normal = normalize(Normal);
    gbuffer_albedo_spec.rgb = texture(u_albedoTexture, TexCoord).rgb;
    //gbuffer_albedo_spec.a = texture(texture_specular1, TexCoord).r;
}