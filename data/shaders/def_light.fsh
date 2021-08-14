#version 330 core
#extension GL_ARB_texture_rectangle: enable

out vec4 fragColor;
in vec2 texCoord;

// G-Buffer
uniform sampler2D u_gbuf_positionTexture;
uniform sampler2D u_gbuf_normalTexture;
uniform sampler2D u_gbuf_colorTexture;

// light stuff
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

// hardcoded but good
vec3 g_ambientLightColor = vec3(0.2, 0.2, 0.2);

struct Light
{
	vec3 pos;
};

vec3 calculatePointLight()
{
	return g_ambientLightColor;
}

void main(void)
{
	// Unpack data from g-buffer
	vec3 pos = texture(u_gbuf_positionTexture, texCoord).rgb;
	vec3 normal = texture(u_gbuf_normalTexture, texCoord).rgb;	
	vec3 color = texture(u_gbuf_colorTexture, texCoord).rgb;
	
	// normalize normal
	normal = normalize(normal);
	
	vec3 resultOfLighting = color * 0.1;
	
	float additionColor = texture(u_gbuf_positionTexture, texCoord).a;
	if (additionColor > 0.0)
	{
		resultOfLighting = color;
		fragColor = vec4(resultOfLighting, 1.0);
	}
	else
	{
		vec3 viewDir = normalize(u_viewPos - pos); 
		vec3 lightDir = normalize(u_lightPos - pos);
		vec3 diffuse = max(dot(normal, lightDir), 0.0) * color * vec3(0.4, 0.4, 0.4); 
		resultOfLighting += diffuse;
	
		fragColor = vec4(resultOfLighting, 1.0);
	}
}