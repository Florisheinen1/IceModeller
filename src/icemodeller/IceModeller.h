#pragma once

#include <GLFW/glfw3.h>
#include <stdexcept>


class IceModeller {
	public:
		IceModeller();

		void start();

	private:
		
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
void character_callback(GLFWwindow* window, unsigned int codepoint);

struct DuplicateInstanceError : public std::runtime_error {
	DuplicateInstanceError();
};