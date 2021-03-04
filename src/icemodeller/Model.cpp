#include <glad/glad.h>

#include <iostream>

#include "Model.h"

const double PI = 3.14159265358979;

Cube::Cube() {
	this->position = glm::vec3(7.0, 0.0, 7.0);
	this->size = glm::vec3(2.0, 1.0, 2.0);

	this->rotation.origin = glm::vec3(8.0, 8.0, 8.0);
	this->rotation.angleDegrees = 0.0f;
	this->rotation.axis = RotationAxis::X;

	this->shade = true;
	
	this->vao = this->createCubeVAO();

	this->faces[0].texture = nullptr;
	this->faces[1].texture = nullptr;
	this->faces[2].texture = nullptr;
	this->faces[3].texture = nullptr;
	this->faces[4].texture = nullptr;
	this->faces[5].texture = nullptr;
}
Cube::Cube(glm::vec3 position, glm::vec3 size, Rotation rotation) {
	this->position = position;
	this->size = size;

	this->rotation = rotation;

	this->shade = false;
	this->vao = this->createCubeVAO();

	this->faces[0].texture = nullptr;
	this->faces[1].texture = nullptr;
	this->faces[2].texture = nullptr;
	this->faces[3].texture = nullptr;
	this->faces[4].texture = nullptr;
	this->faces[5].texture = nullptr;
}
glm::vec3 Cube::getPosition() {
	return this->position;
}
glm::vec3 Cube::getSize() {
	return this->size;
}
Rotation Cube::getRotation() {
	return this->rotation;
}
float Cube::getRotationAngleInRadians() {
	return (float) (this->rotation.angleDegrees * (PI / 180));
}
unsigned int Cube::getVAO() {
	return this->vao;
}
bool Cube::hasTextureAtFace(CubeFace::Orientation orientation) {
	return this->faces[orientation].texture != nullptr;
}

std::vector<float> Cube::getCubeVertices() {
	std::vector<float> vertices = {
		// X			Y				Z					TexX	TexY
		// North face
		-0.5f,			-0.5f,			-0.5f,				1.0f, 0.0f, // A - 0
		size.x - 0.5f,	-0.5f,			-0.5f,				0.0f, 0.0f, // B - 1
		size.x - 0.5f,  size.y - 0.5f,	-0.5f,				0.0f, 1.0f, // F - 2
		-0.5f,			size.y - 0.5f,	-0.5f,				1.0f, 1.0f, // E - 3

		// East face
		size.x - 0.5f,	-0.5f,			-0.5f,				1.0f, 0.0f, // B - 4
		size.x - 0.5f,	-0.5f,			size.z - 0.5f,		0.0f, 0.0f, // C - 5
		size.x - 0.5f,	size.y - 0.5f,	size.z - 0.5f,		0.0f, 1.0f, // G - 6
		size.x - 0.5f,	size.y - 0.5f,	-0.5f,				1.0f, 1.0f, // F - 7

		// South face
		size.x - 0.5f,	-0.5f,			size.z - 0.5f,		1.0f, 0.0f, // C - 8
		-0.5f,			-0.5f,			size.z - 0.5f,		0.0f, 0.0f, // D - 9
		-0.5f,			size.y - 0.5f,	size.z - 0.5f,		0.0f, 1.0f, // H - 10
		size.x - 0.5f,	size.y - 0.5f,	size.z - 0.5f,		1.0f, 1.0f, // G - 11

		// West face
		-0.5f,			-0.5f,			size.z - 0.5f,		1.0f, 0.0f, // D - 12
		-0.5f,			-0.5f,			-0.5f,				0.0f, 0.0f, // A - 13
		-0.5f,			size.y - 0.5f,	-0.5f,				0.0f, 1.0f, // E - 14
		-0.5f,			size.y - 0.5f,	size.z - 0.5f,		1.0f, 1.0f, // H - 15

		// Top face
		-0.5f,			size.y - 0.5f,	-0.5f,				1.0f, 0.0f, // E - 16
		size.x - 0.5f,	size.y - 0.5f,	-0.5f,				0.0f, 0.0f, // F - 17
		size.x - 0.5f,	size.y - 0.5f,	size.z - 0.5f,		0.0f, 1.0f, // G - 18
		-0.5f,			size.y - 0.5f,	size.z - 0.5f,		1.0f, 1.0f, // H - 19

		// Bottom face
		-0.5f,			-0.5f,			size.z - 0.5f,		1.0f, 0.0f, // D - 20
		size.x - 0.5f,	-0.5f,			size.z - 0.5f,		0.0f, 0.0f, // C - 21
		size.x - 0.5f,	-0.5f,			-0.5f,				0.0f, 1.0f, // B - 22
		-0.5f,			-0.5f,			-0.5f,				1.0f, 1.0f, // A - 23
	};
	return vertices;
}
unsigned int Cube::createCubeVAO() {
	const unsigned int cubeIndices[] = {
		2, 1, 0,
		3, 2, 0,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		14, 13, 12,
		15, 14, 12,

		18, 17, 16,
		19, 18, 16,

		22, 21, 20,
		23, 22, 20,
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 120 * sizeof(float), &this->getCubeVertices()[0], GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	return VAO;
}

Model::Model() {
	this->ambientOcclusion = false;
}
std::vector<Cube>* Model::getCubes() {
	return &this->cubes;
}