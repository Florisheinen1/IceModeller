#version 330 core

out vec4 FragColor;

in vec2 textureCoord;

uniform vec4 color;
uniform bool hasTexture;
uniform sampler2D texture1;

void main()
{
	if (hasTexture) {
		FragColor = texture(texture1, textureCoord);
	} else {
		FragColor = color;
	}
}