#pragma once

#include "../gui/Gui.h"
#include "Model.h"

void initialiseModelView();
unsigned int createGridVAO();
std::vector<float> getGridVertices();

class Camera {
	public:
		enum MovementDirection : int {
			MOVE_FORWARD = 0,
			MOVE_BACKWARD = 1,
			MOVE_LEFT = 2,
			MOVE_RIGHT = 3,
			MOVE_UP = 4,
			MOVE_DOWN = 5
		};
		enum RotationDirection : int {
			ROTATE_UP = 0,
			ROTATE_DOWN = 1,
			ROTATE_LEFT = 2,
			ROTATE_RIGHT = 3
		};
		
		Camera();
		Camera(glm::vec3 position, float pitch, float yaw, float fieldOfView);

		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix(int panelWidth, int panelHeight);

		void setMovingInDirection(Camera::MovementDirection direction, bool isMoving);
		void setRotatingInDirection(Camera::RotationDirection direction, bool isRotating);

		void executeMovement();
		void executeRotation();

		void rotate(float addYaw, float addPitch);

		void printData();
	private:
		glm::vec3 position;
		glm::vec3 facingDirection;
		float yaw;
		float pitch;
		float fieldOfView;

		float movementSpeed;
		float rotationSpeed;
		
		bool isMovingInDirection[6] = {false};
		bool isRotatingInDirection[4] = {false};

		static glm::vec3 calculateFacingDirection(float yaw, float pitch);
		static glm::vec3 calculateHorizontalFacingDirection(float yaw);
		static float getValueWithinBounds(float x, float min, float max);
};

class ModelViewPanel : public gui::Element, public gui::FocusListener, public gui::MouseListener, public gui::KeyListener {
	public:
		ModelViewPanel();

		void setViewedModel(Model* model);

		void draw() override;
		void updateBoundary(gui::Boundary boundary);

		void setBackgroundColor(glm::vec4 color);
		void setOpaque(bool isOpaque);

		// Mouse listener methods
		void onMouseEnter() override;
		void onMouseLeave() override;
		void onMousePress(int button, int mods) override;
		void onMouseRelease(int button, int mods) override;
		void onMouseScroll(double horizontalScroll, double verticalScroll) override;

		gui::Boundary getBoundary() override;

		// Key listener methods
		void onCharacterPress(char c) override;
		void onKeyPress(int keyCode, int mods, int action) override;

		// Focus listener methods
		void onFocus();
		void onDefocus();

	private:
		
		int panelX;
		int panelY;
		int panelWidth;
		int panelHeight;

		bool opaque;
		glm::vec4 backgroundColor;

		Model* model;
		Camera camera;

		void drawModel();
		void drawCube(Cube cube);
		void drawGrid();
		static glm::mat4 getModelMatrix(Cube* cube);
};