#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord0;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 Pos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 Tangent;
out vec3 Bitangent;

out mat3 TBN;

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
	Tangent = tangent;
	Bitangent = bitangent;
	
	vec3 T = normalize(vec3(u_model * vec4(tangent,   0.0)));
	vec3 B = normalize(vec3(u_model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(u_model * vec4(normal,    0.0)));
	TBN = mat3(T, B, N);
	
	gl_Position = u_proj * u_view * worldPos;
}