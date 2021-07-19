#version 330 core

layout(location=0) in vec2 position;
layout(location=1) in vec2 texcoord0;

out vec2 attr_texcoord0;

void main()
{
	attr_texcoord0 = texcoord0;
	gl_Position = vec4(position, 1.0, 1.0);
}
