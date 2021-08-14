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
	vec3 dir;
	vec3 color;
	vec3 ambient;
	vec3 specular;
	float shininess;
	float constant;
	float linear;
	float quadratic;
};

uniform Light u_lights[16];
uniform int u_lightsCount;

vec3 calculatePointLight()
{
	return g_ambientLightColor;
}

vec3 pointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
#if 0
    vec3 lightDir = normalize(light.pos - fragPos);
	
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
    // attenuation
    float distance    = length(light.pos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
	#endif
	
	return vec3(1.0);
}

#if 0
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
#endif

vec3 calcPointLight(Light light, vec3 color, vec3 pos, vec3 normal)
{
	vec3 viewDir = normalize(u_viewPos - pos); 
	vec3 lightDir = normalize(light.pos - pos);
	vec3 diffuse = max(dot(normal, lightDir), 0.0) * color * vec3(0.4, 0.4, 0.4); 
	
	return diffuse;
}

void main()
{
	// Unpack data from g-buffer
	vec3 pos = texture(u_gbuf_positionTexture, texCoord).rgb;
	vec3 normal = texture(u_gbuf_normalTexture, texCoord).rgb;	
	vec3 color = texture(u_gbuf_colorTexture, texCoord).rgb;
	
	// then calculate lighting as usual
    vec3 lighting = color * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(u_viewPos - pos);
	
	for(int i = 0; i < u_lightsCount; ++i)
    {
        // diffuse
        //vec3 lightDir = normalize(u_lights[i].pos - pos);
        //vec3 diffuse = max(dot(normal, lightDir), 0.0) * color * u_lights[i].color;
        //lighting += diffuse;
		

		lighting += calcPointLight(u_lights[i], color, pos, normal);
    }
	
	fragColor = vec4(lighting, 1.0);
}