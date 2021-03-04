#pragma once

#include "../gui/Gui.h"

enum class RotationAxis {
	X, Y, Z
};

struct Rotation {
	glm::vec3 origin;
	RotationAxis axis;
	float angleDegrees;

	glm::vec3 getAxisVector() {
		switch (this->axis) {
			case RotationAxis::X:
				return glm::vec3(1.0, 0.0, 0.0);
			case RotationAxis::Y:
				return glm::vec3(0.0, 1.0, 0.0);
			case RotationAxis::Z:
				return glm::vec3(0.0, 0.0, 1.0);
			default:
				return glm::vec3(0.0, 1.0, 0.0);
		}
	}
};

struct Texture {

};

struct CubeFace {
	//uv
	glm::vec2 uvBegin;
	glm::vec2 uvEnd;
	//texture
	Texture* texture;
	int rotation;

	enum Orientation {
		NORTH = 0,
		EAST = 1,
		SOUTH = 2,
		WEST = 3,
		UP = 4,
		DOWN = 5
	};
};


class Cube {
	public:
		Cube();

		Cube(glm::vec3 position, glm::vec3 size, Rotation rotation);

		glm::vec3 getPosition();
		glm::vec3 getSize();
		Rotation getRotation();
		float getRotationAngleInRadians();
		unsigned int getVAO();

		bool hasTextureAtFace(CubeFace::Orientation orientation);
	private:
		unsigned int vao;

		glm::vec3 position;
		glm::vec3 size;

		Rotation rotation;

		bool shade;
		CubeFace faces[6];

		std::vector<float> getCubeVertices();
		unsigned int createCubeVAO();
};


class Model {
	public:
		Model();

		std::vector<Cube>* getCubes();

	private:
		std::vector<Cube> cubes;
		//std::vector<Texture> textures;
		bool ambientOcclusion;

};