// Fragment shader for skinned mesh
#version 330 core

out vec4 fragColor;

// Vertex shader output
in VS_OUTPUT
{
	vec2 texcoord0;
} FS_INPIT;

// texture samplers
uniform sampler2D u_albedoTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_detailTexture;
uniform sampler2D u_roughnessTexture;
uniform sampler2D u_metalnessTexture;
