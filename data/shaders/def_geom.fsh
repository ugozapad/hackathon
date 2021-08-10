// Deferred shading - geometry pass
#version 330 core
layout (location = 0) out vec4 gbuffer_pos;
layout (location = 1) out vec3 gbuffer_normal;
layout (location = 2) out vec4 gbuffer_albedo;

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_albedoTexture;
uniform sampler2D u_normalTexture;
uniform bool u_selfillum;

uniform mat4 u_mv;
uniform mat4 u_mvp;

vec3 calcNormal(vec3 normal, vec2 text_coord)
{
    vec3 newNormal = normal;
    newNormal = texture(u_normalTexture, text_coord).rgb;
    newNormal = normalize(newNormal * 2 - 1) * 2.0;
	newNormal = normalize(u_mv * vec4(newNormal, 0.0)).xyz;
    return newNormal;
}

void main()
{
	// position
	gbuffer_pos.rgb = Pos;

	// calculate normal
	vec3 n = texture(u_normalTexture, TexCoord).rgb;
	n = normalize(n * 2.0 - 1.0);
	
	//gbuffer_normal = normalize(n);
	//gbuffer_normal = normalize( calcNormal( Normal, TexCoord ) );
	gbuffer_normal = Normal;
	
	// little hack for skybox lighting
	if (u_selfillum)
		gbuffer_pos.a = 1.0;
	else
		gbuffer_pos.a = 0.0;
	
	// albedo texture
    gbuffer_albedo.rgb = texture(u_albedoTexture, TexCoord).rgb;
}