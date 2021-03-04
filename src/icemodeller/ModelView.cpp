#include <glad/glad.h>
#include <iostream>
#include <algorithm>
#include "ModelView.h"

#include <gtc/matrix_transform.hpp>

static bool modelViewIsInitialised = false;

const float CAMERA_MAX_YAW = 360.0f;
const float CAMERA_MAX_PITCH = 89.0f;
const float CAMERA_MIN_PITCH = -89.0f;

unsigned int gridVAO;

const glm::vec4 defaultCubeFaceColor[6] = {
	glm::vec4(1.0, 0.0, 0.0, 1.0),	// NORTH	-> RED
	glm::vec4(0.0, 1.0, 0.0, 1.0),	// EAST		-> GREEN
	glm::vec4(0.0, 0.0, 1.0, 1.0),	// SOUTH	-> BLUE
	glm::vec4(1.0, 1.0, 0.0, 1.0),	// WEST		-> YELLOW
	glm::vec4(1.0, 0.0, 1.0, 1.0),	// UP		-> MAGENTA
	glm::vec4(0.0, 1.0, 1.0, 1.0),	// DOWN		-> CYAN
};

const glm::vec3 WORLD_UP = glm::normalize(glm::vec3(0.0, 1.0, 0.0));
Shader* cubeShader;
Shader* lineShader;
Shader* testShader;

void initialiseModelView() {
	std::cout << "Initialising model view" << std::endl;

	if (modelViewIsInitialised)
		throw AlreadyInitializedException();
	if (!gui::Gui::isInitialised())
		throw NotInitializedException();

	lineShader = new Shader("resources/shaders/lineVertex.vs", "resources/shaders/lineFragment.fs");
	cubeShader = new Shader("resources/shaders/cubeVertex.vs", "resources/shaders/cubeFragment.fs");
	testShader = new Shader("resources/shaders/testVertex.vs", "resources/shaders/testFragment.fs");

	gridVAO = createGridVAO();

	modelViewIsInitialised = true;
}

unsigned int createGridVAO() {
	float vertices[] = {
		0.0f, 0.0f, 0.0f,	16.0f, 0.0f, 0.0f,	// East pointer (red)
		0.0f, 0.0f, 0.0f,	0.0f, 16.0f, 0.0f,	// Up pointer (green)
		0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 16.0f,	// South pointer (blue)

		// East-west lines
		0.0f, 0.0f, 1.0f,	16.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 2.0f,	16.0f, 0.0f, 2.0f,
		0.0f, 0.0f, 3.0f,	16.0f, 0.0f, 3.0f,
		0.0f, 0.0f, 4.0f,	16.0f, 0.0f, 4.0f,
		0.0f, 0.0f, 5.0f,	16.0f, 0.0f, 5.0f,
		0.0f, 0.0f, 6.0f,	16.0f, 0.0f, 6.0f,
		0.0f, 0.0f, 7.0f,	16.0f, 0.0f, 7.0f,
		0.0f, 0.0f, 8.0f,	16.0f, 0.0f, 8.0f,
		0.0f, 0.0f, 9.0f,	16.0f, 0.0f, 9.0f,
		0.0f, 0.0f, 10.0f,	16.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 11.0f,	16.0f, 0.0f, 11.0f,
		0.0f, 0.0f, 12.0f,	16.0f, 0.0f, 12.0f,
		0.0f, 0.0f, 13.0f,	16.0f, 0.0f, 13.0f,
		0.0f, 0.0f, 14.0f,	16.0f, 0.0f, 14.0f,
		0.0f, 0.0f, 15.0f,	16.0f, 0.0f, 15.0f,
		0.0f, 0.0f, 16.0f,	16.0f, 0.0f, 16.0f,

		// North-south lines
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 16.0f,
		2.0f, 0.0f, 0.0f,	2.0f, 0.0f, 16.0f,
		3.0f, 0.0f, 0.0f,	3.0f, 0.0f, 16.0f,
		4.0f, 0.0f, 0.0f,	4.0f, 0.0f, 16.0f,
		5.0f, 0.0f, 0.0f,	5.0f, 0.0f, 16.0f,
		6.0f, 0.0f, 0.0f,	6.0f, 0.0f, 16.0f,
		7.0f, 0.0f, 0.0f,	7.0f, 0.0f, 16.0f,
		8.0f, 0.0f, 0.0f,	8.0f, 0.0f, 16.0f,
		9.0f, 0.0f, 0.0f,	9.0f, 0.0f, 16.0f,
		10.0f, 0.0f, 0.0f,	10.0f, 0.0f, 16.0f,
		11.0f, 0.0f, 0.0f,	11.0f, 0.0f, 16.0f,
		12.0f, 0.0f, 0.0f,	12.0f, 0.0f, 16.0f,
		13.0f, 0.0f, 0.0f,	13.0f, 0.0f, 16.0f,
		14.0f, 0.0f, 0.0f,	14.0f, 0.0f, 16.0f,
		15.0f, 0.0f, 0.0f,	15.0f, 0.0f, 16.0f,
		16.0f, 0.0f, 0.0f,	16.0f, 0.0f, 16.0f,
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return VAO;
}

/*
unsigned int createGridVAO() {
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float* vertices = &getGridVertices()[0];
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	return VAO;
}

std::vector<float> getGridVertices() {
	std::vector<float> vertices = {
		0.0f, 0.0f, 0.0f,		16.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,		16.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 2.0f,		16.0f, 0.0f, 2.0f,
		0.0f, 0.0f, 3.0f,		16.0f, 0.0f, 3.0f,
		0.0f, 0.0f, 4.0f,		16.0f, 0.0f, 4.0f,
		0.0f, 0.0f, 5.0f,		16.0f, 0.0f, 5.0f,
		0.0f, 0.0f, 6.0f,		16.0f, 0.0f, 6.0f,
		0.0f, 0.0f, 7.0f,		16.0f, 0.0f, 7.0f,
		0.0f, 0.0f, 8.0f,		16.0f, 0.0f, 8.0f,
		0.0f, 0.0f, 9.0f,		16.0f, 0.0f, 9.0f,
		0.0f, 0.0f, 10.0f,		16.0f, 0.0f, 10.0f,
		0.0f, 0.0f, 11.0f,		16.0f, 0.0f, 11.0f,
		0.0f, 0.0f, 12.0f,		16.0f, 0.0f, 12.0f,
		0.0f, 0.0f, 13.0f,		16.0f, 0.0f, 13.0f,
		0.0f, 0.0f, 14.0f,		16.0f, 0.0f, 14.0f,
		0.0f, 0.0f, 15.0f,		16.0f, 0.0f, 15.0f,
		0.0f, 0.0f, 16.0f,		16.0f, 0.0f, 16.0f,

		0.0f, 0.0f, 0.0f,		0.0f, 0.0f, 16.0f,
		1.0f, 0.0f, 0.0f,		1.0f, 0.0f, 16.0f,
		2.0f, 0.0f, 0.0f,		2.0f, 0.0f, 16.0f,
		3.0f, 0.0f, 0.0f,		3.0f, 0.0f, 16.0f,
		4.0f, 0.0f, 0.0f,		4.0f, 0.0f, 16.0f,
		5.0f, 0.0f, 0.0f,		5.0f, 0.0f, 16.0f,
		6.0f, 0.0f, 0.0f,		6.0f, 0.0f, 16.0f,
		7.0f, 0.0f, 0.0f,		7.0f, 0.0f, 16.0f,
		8.0f, 0.0f, 0.0f,		8.0f, 0.0f, 16.0f,
		9.0f, 0.0f, 0.0f,		9.0f, 0.0f, 16.0f,
		10.0f, 0.0f, 0.0f,		10.0f, 0.0f, 16.0f,
		11.0f, 0.0f, 0.0f,		11.0f, 0.0f, 16.0f,
		12.0f, 0.0f, 0.0f,		12.0f, 0.0f, 16.0f,
		13.0f, 0.0f, 0.0f,		13.0f, 0.0f, 16.0f,
		14.0f, 0.0f, 0.0f,		14.0f, 0.0f, 16.0f,
		15.0f, 0.0f, 0.0f,		15.0f, 0.0f, 16.0f,
		16.0f, 0.0f, 0.0f,		16.0f, 0.0f, 16.0f
	};

	return vertices;
}*/

// Camera
Camera::Camera() {
	this->position = glm::vec3(10.0, 2.0, 0.0);
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	this->fieldOfView = 90.0f;
	this->movementSpeed = 0.1f;
	this->rotationSpeed = 1.0f;

	this->facingDirection = Camera::calculateFacingDirection(this->yaw, this->pitch);

	std::cout << "Constructor: ";
	this->printData();
}
Camera::Camera(glm::vec3 position, float yaw, float pitch, float fiedOfView) {
	this->position = position;
	this->yaw = yaw;
	this->pitch = pitch;
	this->fieldOfView = fieldOfView;
	this->movementSpeed = 0.1f;
	this->rotationSpeed = 1.0f;

	this->facingDirection = Camera::calculateFacingDirection(this->yaw, this->pitch);

}
glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(
		this->position,
		this->position + this->facingDirection,
		WORLD_UP);
}
glm::mat4 Camera::getProjectionMatrix(int panelWidth, int panelHeight) {
	return glm::perspective(glm::radians(this->fieldOfView), (float)panelWidth / (float)panelHeight, 0.02f, 1000.0f);
}
void Camera::setMovingInDirection(Camera::MovementDirection direction, bool isMoving) {
	this->isMovingInDirection[direction] = isMoving;
}
void Camera::setRotatingInDirection(Camera::RotationDirection direction, bool isRotating) {
	this->isRotatingInDirection[direction] = isRotating;
}
glm::vec3 Camera::calculateFacingDirection(float yaw, float pitch) {
	return -glm::vec3(
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	);
}
glm::vec3 Camera::calculateHorizontalFacingDirection(float yaw) {
	return glm::vec3(cos(glm::radians(yaw)),
					0,
					sin(glm::radians(yaw)));
}
void Camera::executeMovement() {
	glm::vec3 horizontalFacingVector = glm::normalize(Camera::calculateHorizontalFacingDirection(this->yaw));
	glm::vec3 leftVector = glm::vec3(-horizontalFacingVector.z, 0, horizontalFacingVector.x);

	glm::vec3 movementSum = glm::vec3(0.0, 0.0, 0.0);

	if (this->isMovingInDirection[MovementDirection::MOVE_FORWARD])
		movementSum -= horizontalFacingVector;
	if (this->isMovingInDirection[MovementDirection::MOVE_BACKWARD])
		movementSum += horizontalFacingVector;
	if (this->isMovingInDirection[MovementDirection::MOVE_LEFT])
		movementSum += leftVector;
	if (this->isMovingInDirection[MovementDirection::MOVE_RIGHT])
		movementSum -= leftVector;
	if (this->isMovingInDirection[MovementDirection::MOVE_UP])
		movementSum += WORLD_UP;
	if (this->isMovingInDirection[MovementDirection::MOVE_DOWN])
		movementSum -= WORLD_UP;

	if (glm::length(movementSum) >= 1) {
		this->position += this->movementSpeed * glm::normalize(movementSum);
	}
}
void Camera::executeRotation() {
	float yawOffset = 0.0f;
	float pitchOffset = 0.0f;
	if (this->isRotatingInDirection[RotationDirection::ROTATE_LEFT])
		yawOffset -= this->rotationSpeed;
	if (this->isRotatingInDirection[RotationDirection::ROTATE_RIGHT])
		yawOffset += this->rotationSpeed;
	if (this->isRotatingInDirection[RotationDirection::ROTATE_UP])
		pitchOffset -= this->rotationSpeed;
	if (this->isRotatingInDirection[RotationDirection::ROTATE_DOWN])
		pitchOffset += this->rotationSpeed;

	if (yawOffset != 0.0f || pitchOffset != 0.0f) {
		this->rotate(yawOffset, pitchOffset);
	}
}
void Camera::rotate(float addYaw, float addPitch) { 
	this->yaw = fmod((this->yaw + addYaw), CAMERA_MAX_YAW);
	this->pitch = Camera::getValueWithinBounds(this->pitch + addPitch, CAMERA_MIN_PITCH, CAMERA_MAX_PITCH);
	this->facingDirection = Camera::calculateFacingDirection(this->yaw, this->pitch);

	//this->printData();
}
void Camera::printData() {
	std::cout << "Cam: Y: " << this->yaw << ", P: " << this->pitch << ", lookAt: [" << this->facingDirection.x << ", " << this->facingDirection.y << ", " << this->facingDirection.z << "]" << std::endl;
}
float Camera::getValueWithinBounds(float x, float min, float max) {
	return std::min(std::max(x, min), max);
}
// Model view
ModelViewPanel::ModelViewPanel() {
	if (!modelViewIsInitialised)
		throw NotInitializedException();

	this->panelX = 0;
	this->panelY = 0;
	this->panelWidth = 0;
	this->panelHeight = 0;

	this->opaque = false;
	this->backgroundColor = glm::vec4(0.0, 0.0, 0.0, 1.0);

	this->model = nullptr;
	this->camera = Camera();

	gui::Gui::keyManager->registerKeyListener(this);
}

void ModelViewPanel::setViewedModel(Model* model) {
	this->model = model;
}

void ModelViewPanel::draw() {
	//glClear(GL_COLOR_BUFFER_BIT);
	this->camera.executeMovement();
	this->camera.executeRotation();

	// Draw background with gui shader if necessary
	if (this->opaque) {
		gui::Gui::shader->use();
		gui::Gui::shader->setMat4("transformation", this->transformationMatrix);
		gui::Gui::shader->setBool("hasTexture", false);
		gui::Gui::shader->setBool("isTextureColored", false);
		gui::Gui::shader->setVec4("color", this->backgroundColor);

		glBindVertexArray(gui::Gui::quadVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Set up viewport
	GLint originalViewport[4];
	glGetIntegerv(GL_VIEWPORT, originalViewport);

	//std::cout << "Viewport: " << this->panelX << ", " << this->panelY << ", " << this->panelWidth << ", " << this->panelHeight << std::endl;
	glViewport(this->panelX, this->panelY, this->panelWidth, this->panelHeight);

	this->drawModel();
	this->drawGrid();

	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	// Restore original viewport
	glViewport(originalViewport[0], originalViewport[1], originalViewport[2], originalViewport[3]);
}
void ModelViewPanel::updateBoundary(gui::Boundary boundary) {
	this->boundary = boundary;
	this->transformationMatrix = gui::Gui::getTransformationMatrix(this->boundary);

	this->panelX = (int) ((boundary.left + 1.0f) / 2.0f * gui::Gui::mainWindow->getWindowWidth());
	this->panelY = (int) ((boundary.bottom + 1.0f) / 2.0f * gui::Gui::mainWindow->getWindowHeight());

	float screenWidth = this->boundary.right - this->boundary.left;
	float screenHeight = this->boundary.top - this->boundary.bottom;

	this->panelWidth = (int) ((screenWidth / 2.0f) * gui::Gui::mainWindow->getWindowWidth());
	this->panelHeight = (int) ((screenHeight / 2.0f) * gui::Gui::mainWindow->getWindowHeight());
}
void ModelViewPanel::setOpaque(bool isOpaque) {
	this->opaque = isOpaque;
}
void ModelViewPanel::setBackgroundColor(glm::vec4 color) {
	this->backgroundColor = color;
}

void ModelViewPanel::onMouseEnter() {

}
void ModelViewPanel::onMouseLeave() {

}
void ModelViewPanel::onMousePress(int button, int mods) {

}
void ModelViewPanel::onMouseRelease(int button, int mods) {

}
void ModelViewPanel::onMouseScroll(double horizontalScroll, double verticalScroll) {

}
gui::Boundary ModelViewPanel::getBoundary() {
	return this->boundary;
}
void ModelViewPanel::onCharacterPress(char c) {}
void ModelViewPanel::onKeyPress(int keyCode, int mods, int action) {
	bool isPressed = action >= 1;

	switch (keyCode) {
		case GLFW_KEY_W:
			this->camera.setMovingInDirection(Camera::MovementDirection::MOVE_FORWARD, isPressed);
			break;
		case GLFW_KEY_A:
			this->camera.setMovingInDirection(Camera::MovementDirection::MOVE_LEFT, isPressed);
			break;
		case GLFW_KEY_S:
			this->camera.setMovingInDirection(Camera::MovementDirection::MOVE_BACKWARD, isPressed);
			break;
		case GLFW_KEY_D:
			this->camera.setMovingInDirection(Camera::MovementDirection::MOVE_RIGHT, isPressed);
			break;
		case GLFW_KEY_SPACE:
			this->camera.setMovingInDirection(Camera::MovementDirection::MOVE_UP, isPressed);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			this->camera.setMovingInDirection(Camera::MovementDirection::MOVE_DOWN, isPressed);
			break;
		case GLFW_KEY_LEFT:
			this->camera.setRotatingInDirection(Camera::RotationDirection::ROTATE_LEFT, isPressed);
			break;
		case GLFW_KEY_RIGHT:
			this->camera.setRotatingInDirection(Camera::RotationDirection::ROTATE_RIGHT, isPressed);
			break;
		case GLFW_KEY_UP:
			this->camera.setRotatingInDirection(Camera::RotationDirection::ROTATE_UP, isPressed);
			break;
		case GLFW_KEY_DOWN:
			this->camera.setRotatingInDirection(Camera::RotationDirection::ROTATE_DOWN, isPressed);
			break;

	}
}
void ModelViewPanel::onFocus() {

}
void ModelViewPanel::onDefocus() {

}

void ModelViewPanel::drawModel() {
	if (this->model == nullptr)
		return;

	cubeShader->use();
	// Initialise camera position and view into shader
	glm::mat4 projectionMatrix = this->camera.getProjectionMatrix(this->panelWidth, this->panelHeight);
	cubeShader->setMat4("projectionMatrix", projectionMatrix);
	glm::mat4 viewMatrix = this->camera.getViewMatrix();
	cubeShader->setMat4("viewMatrix", viewMatrix);

	std::vector<Cube>* cubes = this->model->getCubes();
	for (auto it = cubes->begin(); it != cubes->end(); ++it) {
		Cube cube = *it;

		this->drawCube(cube);
	}
}
void ModelViewPanel::drawCube(Cube cube) {
	// Get matrices
	glm::mat4 modelMatrix = ModelViewPanel::getModelMatrix(&cube);

	cubeShader->setMat4("modelMatrix", modelMatrix);
	glBindVertexArray(cube.getVAO());

	// North face
	if (!cube.hasTextureAtFace(CubeFace::Orientation::NORTH)) {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", defaultCubeFaceColor[CubeFace::Orientation::NORTH]);
	} else {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// East face
	if (!cube.hasTextureAtFace(CubeFace::Orientation::EAST)) {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", defaultCubeFaceColor[CubeFace::Orientation::EAST]);
	} else {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(float)));

	// South face
	if (!cube.hasTextureAtFace(CubeFace::Orientation::SOUTH)) {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", defaultCubeFaceColor[CubeFace::Orientation::SOUTH]);
	} else {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(float)));

	// West face
	if (!cube.hasTextureAtFace(CubeFace::Orientation::WEST)) {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", defaultCubeFaceColor[CubeFace::Orientation::WEST]);
	} else {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(float)));

	// Up face
	if (!cube.hasTextureAtFace(CubeFace::Orientation::UP)) {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", defaultCubeFaceColor[CubeFace::Orientation::UP]);
	} else {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(float)));

	// Down face
	if (!cube.hasTextureAtFace(CubeFace::Orientation::DOWN)) {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", defaultCubeFaceColor[CubeFace::Orientation::DOWN]);
	} else {
		cubeShader->setBool("hasTexture", false);
		cubeShader->setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
	}
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(float)));
}
void ModelViewPanel::drawGrid() {
	lineShader->use();
	
	glm::mat4 projectionMatrix = this->camera.getProjectionMatrix(this->panelWidth, this->panelHeight);
	lineShader->setMat4("projectionMatrix", projectionMatrix);
	glm::mat4 viewMatrix = this->camera.getViewMatrix();
	lineShader->setMat4("viewMatrix", viewMatrix);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	lineShader->setMat4("modelMatrix", modelMatrix);

	glBindVertexArray(gridVAO);

	// Draw x axis
	lineShader->setVec4("color", glm::vec4(1.0, 0.0, 0.0, 1.0));
	glDrawArrays(GL_LINES, 0, 2);

	// Draw y axis
	lineShader->setVec4("color", glm::vec4(0.0, 1.0, 0.0, 1.0));
	glDrawArrays(GL_LINES, 2, 2);

	// Draw z axis
	lineShader->setVec4("color", glm::vec4(0.0, 0.0, 1.0, 1.0));
	glDrawArrays(GL_LINES, 4, 2);

	// Draw rest of grid
	lineShader->setVec4("color", glm::vec4(1.0, 1.0, 1.0, 1.0));
	glDrawArrays(GL_LINES, 6, 70);
}
glm::mat4 ModelViewPanel::getModelMatrix(Cube* cube) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	// Move to pivot point
	modelMatrix = glm::translate(modelMatrix, cube->getRotation().origin);
	// Rotate around pivot point
	modelMatrix = glm::rotate(modelMatrix, cube->getRotationAngleInRadians(), cube->getRotation().getAxisVector());
	// Move back from pivot point
	modelMatrix = glm::translate(modelMatrix, -cube->getRotation().origin);

	// Align cube with grid
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5, 0.5, 0.5));

	// Move to position
	modelMatrix = glm::translate(modelMatrix, cube->getPosition());

	return modelMatrix;
}