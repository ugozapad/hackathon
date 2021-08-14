#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord0;

out vec3 Pos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	vec4 worldPos = u_model * vec4(position, 1.0);
	Pos = worldPos.xyz;
	
	mat3 normalMatrix = transpose(inverse(mat3(u_model)));
	Normal = normalMatrix * normal;
	
	TexCoord = texcoord0;
	
	gl_Position = u_proj * u_view * worldPos;
}