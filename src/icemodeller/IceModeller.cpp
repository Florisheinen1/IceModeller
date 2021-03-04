#include <GLFW/glfw3.h>
#include <iostream>
#include <glm.hpp>
#include "IceModeller.h"
#include "../gui/Gui.h"
#include "ModelView.h"

static const int DEFAULT_WINDOW_WIDTH = 1024;
static const int DEFAULT_WINDOW_HEIGHT = 571;
static const int MINIMAL_WINDOW_WIDTH = 1024;
static const int MINIMAL_WINDOW_HEIGHT = 571;
static const char* TITLE = "IceModeller v1.0";

static const glm::vec4 BACKGROUND_COLOR = glm::vec4(0.12, 0.12, 0.12, 1.0);
static const glm::vec4 BACKGROUND_COLOR2 = glm::vec4(0.153, 0.153, 0.153, 1.0);

IceModeller::IceModeller() {

	std::cout << "1. Initialising IceModeller" << std::endl;

	gui::Gui::init(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

	initialiseModelView();

	//oglWindow = new gui::OGLWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	gui::Gui::mainWindow->setMinimalWindowSize(MINIMAL_WINDOW_WIDTH, MINIMAL_WINDOW_HEIGHT);
	gui::Gui::mainWindow->setBackgroundColor(BACKGROUND_COLOR);
	gui::Gui::mainWindow->setTitle(TITLE);

	gui::Gui::mainWindow->setLayout(new gui::BorderLayout());

	gui::Panel* menuBar = new gui::Panel();
	menuBar->setLayout(new gui::AbsoluteLayout());
	menuBar->preferredDimension.height = 30;
	menuBar->preferredSide = gui::Side::NORTH;
	gui::Gui::mainWindow->addElement(menuBar);

	gui::Panel* controlPanel = new gui::Panel();
	//controlPanel->setLayout(new gui::AbsoluteLayout());
	controlPanel->setLayout(new gui::BorderLayout());
	controlPanel->preferredDimension.width = 421;
	controlPanel->preferredSide = gui::Side::EAST;

	controlPanel->setBackgroundColor(glm::vec4(25.0f /255, 25.0f/255, 25.0f/255, 1.0));
	controlPanel->setOpaque(true);

	gui::Gui::mainWindow->addElement(controlPanel);

	gui::Panel* positionPanel = new gui::Panel();
	positionPanel->setBackgroundColor(BACKGROUND_COLOR2);
	positionPanel->setLayout(new gui::AbsoluteLayout());
	positionPanel->setOpaque(true);
	positionPanel->preferredSide = gui::Side::EAST;
	positionPanel->preferredDimension.width = 224;
	//positionPanel->setPreferredDimension(231, 6, 184, 559);
	controlPanel->addElement(positionPanel);

	unsigned int textureButton = gui::Gui::loadTexture("resources/sprites/button.png");
	unsigned int textureButtonHover = gui::Gui::loadTexture("resources/sprites/buttonHover.png");
	unsigned int textureButtonPress = gui::Gui::loadTexture("resources/sprites/buttonPress.png");

	unsigned int textureButtonFlipped = gui::Gui::loadTexture("resources/sprites/buttonFlipped.png");
	unsigned int textureButtonHoverFlipped = gui::Gui::loadTexture("resources/sprites/buttonHoverFlipped.png");
	unsigned int textureButtonPressFlipped = gui::Gui::loadTexture("resources/sprites/buttonPressFlipped.png");

	// Input textures
	unsigned int inputTexture = gui::Gui::loadTexture("resources/sprites/input.png");
	unsigned int inputTextureHovered = gui::Gui::loadTexture("resources/sprites/inputHovered.png");
	unsigned int inputTextureFocused = gui::Gui::loadTexture("resources/sprites/inputFocused.png");

	// Size
	gui::DoubleInput* sizeX = new gui::DoubleInput();
	sizeX->setPreferredDimension(2, 60, 72, 25);
	sizeX->setFontSize(12);
	sizeX->setBackgroundImage(inputTexture, inputTextureHovered, inputTextureFocused);
	sizeX->setColor(glm::vec4(0.8, 0.8, 0.8, 0.8));
	sizeX->setTextOffset(5, 5);
	positionPanel->addElement(sizeX);

	gui::DoubleInput* sizeY = new gui::DoubleInput();
	sizeY->setPreferredDimension(76, 60, 72, 25);
	sizeY->setFontSize(12);
	sizeY->setBackgroundImage(inputTexture, inputTextureHovered, inputTextureFocused);
	sizeY->setColor(glm::vec4(0.8, 0.8, 0.8, 0.8));
	sizeY->setTextOffset(5, 5);
	positionPanel->addElement(sizeY);

	gui::DoubleInput* sizeZ = new gui::DoubleInput();
	sizeZ->setPreferredDimension(150, 60, 72, 25);
	sizeZ->setFontSize(12);
	sizeZ->setBackgroundImage(inputTexture, inputTextureHovered, inputTextureFocused);
	sizeZ->setColor(glm::vec4(0.8, 0.8, 0.8, 0.8));
	sizeZ->setTextOffset(5, 5);
	positionPanel->addElement(sizeZ);

	gui::Label* sizeLabel = new gui::Label("Size");
	sizeLabel->setColor(glm::vec4(172.0f / 255, 171.0f / 255, 171.0f / 255, 1.0));
	sizeLabel->setFontSize(16);
	sizeLabel->setPreferredDimension(10, 5, 50, 16);
	positionPanel->addElement(sizeLabel);

	gui::IncrementButton* buttonSizeIncreaseX = new gui::IncrementButton(textureButton, textureButtonHover, textureButtonPress);
	buttonSizeIncreaseX->setPreferredDimension(2, 27, 72, 32);
	buttonSizeIncreaseX->setIncrementListener(sizeX, 1.0, 0.1, 0.01, 0.001);
	positionPanel->addElement(buttonSizeIncreaseX);

	gui::IncrementButton* buttonSizeIncreaseY = new gui::IncrementButton(textureButton, textureButtonHover, textureButtonPress);
	buttonSizeIncreaseY->setPreferredDimension(76, 27, 72, 32);
	buttonSizeIncreaseY->setIncrementListener(sizeY, 1.0, 0.1, 0.01, 0.001);
	positionPanel->addElement(buttonSizeIncreaseY);

	gui::IncrementButton* buttonSizeIncreaseZ = new gui::IncrementButton(textureButton, textureButtonHover, textureButtonPress);
	buttonSizeIncreaseZ->setPreferredDimension(150, 27, 72, 32);
	buttonSizeIncreaseZ->setIncrementListener(sizeZ, 1.0, 0.1, 0.01, 0.001);
	positionPanel->addElement(buttonSizeIncreaseZ);

	gui::IncrementButton* buttonSizeDecreaseX = new gui::IncrementButton(textureButtonFlipped, textureButtonHoverFlipped, textureButtonPressFlipped);
	buttonSizeDecreaseX->setPreferredDimension(2, 86, 72, 32);
	buttonSizeDecreaseX->setIncrementListener(sizeX, -1.0, -0.1, -0.01, -0.001);
	positionPanel->addElement(buttonSizeDecreaseX);

	gui::IncrementButton* buttonSizeDecreaseY = new gui::IncrementButton(textureButtonFlipped, textureButtonHoverFlipped, textureButtonPressFlipped);
	buttonSizeDecreaseY->setPreferredDimension(76, 86, 72, 32);
	buttonSizeDecreaseY->setIncrementListener(sizeY, -1.0, -0.1, -0.01, -0.001);
	positionPanel->addElement(buttonSizeDecreaseY);

	gui::IncrementButton* buttonSizeDecreaseZ = new gui::IncrementButton(textureButtonFlipped, textureButtonHoverFlipped, textureButtonPressFlipped);
	buttonSizeDecreaseZ->setPreferredDimension(150, 86, 72, 32);
	buttonSizeDecreaseZ->setIncrementListener(sizeZ, -1.0, -0.1, -0.01, -0.001);
	positionPanel->addElement(buttonSizeDecreaseZ);

	// Position
	gui::Label* positionLabel = new gui::Label("Position");
	positionLabel->setColor(glm::vec4(172.0f / 255, 171.0f / 255, 171.0f / 255, 1.0));
	positionLabel->setFontSize(40);
	positionLabel->setPreferredDimension(10, 138, 100, 16);
	positionPanel->addElement(positionLabel);

	Model* testModel = new Model();
	Cube tCube1 = Cube(glm::vec3(4.0, 0.0, 7.0), glm::vec3(2.0, 1.0, 2.0), Rotation{glm::vec3(0.0, 0.0, 0.0), RotationAxis::X, 0.0f});
	testModel->getCubes()->push_back(tCube1);

	Cube tCube2 = Cube(glm::vec3(13.0, 2.0, 4.0), glm::vec3(2.0, 1.0, 2.0), Rotation{glm::vec3(0.0, 0.0, 0.0), RotationAxis::X, 0.0f});
	testModel->getCubes()->push_back(tCube2);

	//Cube tCube3 = Cube(glm::vec3(0.0, 0.0, 0.0), glm::vec3(16.0, 0.5, 16.0), Rotation{glm::vec3(0.0, 0.0, 0.0), RotationAxis::X, 10.0f});
	//testModel->getCubes()->push_back(tCube3);

	Cube tCube4 = Cube(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), Rotation{glm::vec3(0.5, 0.5, 0.5), RotationAxis::Y, 45.0f});
	testModel->getCubes()->push_back(tCube4);

	ModelViewPanel* modelView = new ModelViewPanel();
	modelView->preferredSide = gui::Side::CENTER;
	modelView->setBackgroundColor(glm::vec4(0.7, 0.7, 0.7, 1.0));
	modelView->setOpaque(true);
	modelView->setViewedModel(testModel);
	gui::Gui::mainWindow->addElement(modelView);

	//gui::Panel* modelView = new gui::Panel();
	//modelView->preferredSide = gui::Side::CENTER;
	//modelView->setBackgroundColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
	//modelView->setOpaque(true);
	//oglWindow->addElement(modelView);
}


void IceModeller::start() {
	while (!gui::Gui::mainWindow->shoudClose()) {
		gui::Gui::mainWindow->draw();
	}
}
void framebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
	gui::Gui::mainWindow->updateWindowSize(width, height);
}
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	std::cout << "Scrolled" << std::endl;
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	gui::Gui::mouseManager->onMouseClick(button, action, mods);
}
void character_callback(GLFWwindow* window, unsigned int codepoint) {

}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (mods == 0) {
			switch (key) {
				case GLFW_KEY_TAB:
					std::cout << "Tab!" << std::endl;
					break;
				case GLFW_KEY_ESCAPE:
					std::cout << "Escape!" << std::endl;
					break;
			}
		} else if (mods == GLFW_MOD_CONTROL) {
			switch (key) {
				case GLFW_KEY_C:
					std::cout << "Copy!" << std::endl;
					break;
				case GLFW_KEY_S:
					std::cout << "Save!" << std::endl;
					break;
				case GLFW_KEY_X:
					std::cout << "Cut!" << std::endl;
					break;
				case GLFW_KEY_V:
					std::cout << "Paste!" << std::endl;
					break;
				case GLFW_KEY_Z:
					std::cout << "Undo!" << std::endl;
					break;
				case GLFW_KEY_O:
					std::cout << "Open!" << std::endl;
					break;
			}
		} else if (mods == (GLFW_MOD_CONTROL | GLFW_MOD_SHIFT)) {
			switch (key) {
				case GLFW_KEY_Z:
					std::cout << "Redo!" << std::endl;
					break;
			}
		}
	}
}

DuplicateInstanceError::DuplicateInstanceError() : std::runtime_error("Already initialized a window") {}