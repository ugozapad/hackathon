// Deferred shading - geometry pass
#version 330 core
layout (location = 0) out vec4 gbuffer_pos;
layout (location = 1) out vec3 gbuffer_normal;
layout (location = 2) out vec4 gbuffer_albedo;

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;
in vec3 Bitangent;
in mat3 TBN;

uniform sampler2D u_albedoTexture;
uniform sampler2D u_normalTexture;
uniform bool u_selfillum;

uniform mat4 u_model;
uniform mat4 u_view;

vec3 calcNormal(vec3 normal, vec2 tex_coord)
{
	vec3 n = texture(u_normalTexture, tex_coord).xyz * 2.0 - 1.0;
	n = TBN * n;
	n = normalize(n);
	n = n * 0.5 + 0.5;
	return n;
}

void main()
{
	// position
	gbuffer_pos.rgb = Pos;

	// calculate normal
	//vec3 n = texture(u_normalTexture, TexCoord).rgb;
	//n = normalize(n * 2.0 - 1.0);
	
	//gbuffer_normal = normalize(n);
	gbuffer_normal = calcNormal(Normal, TexCoord);
	//gbuffer_normal = Normal;
	
	// little hack for skybox lighting
	if (u_selfillum)
		gbuffer_pos.a = 1.0;
	else
		gbuffer_pos.a = 0.0;
	
	// albedo texture
    gbuffer_albedo.rgb = texture(u_albedoTexture, TexCoord).rgb;
	
	//float gamma = 2.2;
	//vec3 diffuseColor = pow(texture(u_albedoTexture, TexCoord).rgb, vec3(gamma));
	//gbuffer_albedo.rgb = diffuseColor;
}