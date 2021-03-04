#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoordIn;

out vec2 textureCoord;

uniform mat4 transformation;

void main()
{
	textureCoord = textureCoordIn;
	gl_Position = transformation * vec4(position, 0.0, 1.0);
}