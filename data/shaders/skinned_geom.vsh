// Vertex shader for skinned model
#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord0;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

// Vertex shader output
out VS_OUTPUT
{
	vec2 texcoord0;
} VS_OUT;

void main()
{
	mat4 mvp = u_proj * u_view * u_model;
	
	// #TODO: bone matrix translation
	gl_Position = mvp * vec4(position, 1.0);

	// Output to fragment shader
	VS_OUT.texcoord0 = texcoord0;
}