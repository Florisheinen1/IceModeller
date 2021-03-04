#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include "../import/Shader.h"

namespace gui {

	struct Range {
		int index;
		int length;
	};

	enum class Side {
		CENTER,
		NORTH,
		EAST,
		SOUTH,
		WEST
	};

	struct Dimension {
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	};

	struct Boundary {
		float top = 0.0f;
		float right = 0.0f;
		float bottom = 0.0f;
		float left = 0.0f;

		bool contains(float x, float y);

		void print(bool endLine);
	};

	// Listeners
	class MouseListener {
		public:
			virtual void onMouseEnter();
			virtual void onMouseLeave();
			virtual void onMousePress(int button, int mods);
			virtual void onMouseRelease(int button, int mods);
			virtual void onMouseScroll(double horizontalScroll, double verticalScroll);
			
			virtual Boundary getBoundary() = 0;
	};
	class KeyListener {
		public:
			virtual void onCharacterPress(char c) = 0;
			virtual void onKeyPress(int keycode, int mods, int action) = 0;
	};
	class FocusListener {
		public:
			FocusListener();

			virtual void onFocus();
			virtual void onDefocus();

			void setPreviousElement(FocusListener* previousElementsFocusListener);
		private:
			FocusListener* nextElementsFocusListener;
			FocusListener* previousElementsFocusListener;
	};

	class Element {
		public:
			Element();

			virtual void draw() = 0;
			virtual void updateBoundary(Boundary boundary);
			void setPreferredDimension(int x, int y, int width, int height);

			Dimension preferredDimension;
			Side preferredSide;
	protected:
			Boundary boundary;
			glm::mat4 transformationMatrix;
	};

	class Layout {
		public:
			virtual void updateBoundaries(std::vector<Element*>* elements, Boundary boundary) = 0;
	};

	class BorderLayout : public Layout {
		public:
			void updateBoundaries(std::vector<Element*>* elements, Boundary boundary) override;
	};

	class AbsoluteLayout : public Layout {
		public:
			void updateBoundaries(std::vector<Element*>* elements, Boundary boundary) override;
	};

	class Container : public Element {
		public:
			Container();

			void addElement(Element* element);
			void setLayout(Layout* layout);

		protected:
			std::vector<Element*> children;
			Layout* layout;
	};

	class Panel : public Container {
		public:
			Panel();

			void draw() override;
			void updateBoundary(Boundary boundary) override;

			void setOpaque(bool opaque);
			void setBackgroundColor(glm::vec4 color);
			void setBackgroundImage(unsigned int textureID);
		protected:
			glm::vec4 backgroundColor;
		private:
			bool opaque;
			bool hasTexture;
			unsigned int textureID;
	};

	class TextLine {
		public:
			TextLine(std::string text);
			TextLine(std::string text, int fontSize, float charDistance, glm::vec4 color);

			void drawCharacters();

			void setColor(glm::vec4 color);
			void setFontSize(int fontSize);
			int getFontSize();
			int getInsertPositionAtMouse();
			void drawInsertLine(int position);
		protected:
			void setTextPosition(float screenLeft, float screenTop);
			float getCharDistance();
			glm::vec4 getColor();
			std::string text;
		private:
			int fontSize;
			float charDistance;
			glm::vec4 color;

			float screenTextLeft;
			float screenTextTop;
	};

	class Label : public Element, public TextLine {
		public:
			Label(std::string text);

			void draw() override;

			void updateBoundary(Boundary boundary) override;

			void setOpaque(bool opaque);
			void setBackgroundColor(glm::vec4 color);
			void setBackgroundImage(unsigned int image);
			void setTextOffset(int left, int top);
		private:
			
			void updateTextPosition();

			int textOffsetLeft;
			int textOffsetTop;

			bool opaque;
			bool usesTexture;
			glm::vec4 backgroundColor;
			unsigned int backgroundImage;
	};

	class IncrementListener {
		public:
			virtual void increase(double increment) = 0;
	};

	class Button : public Element, public MouseListener {
		public:
			Button(unsigned int texture, unsigned int textureHover, unsigned int texturePress);

			void draw() override;
			void updateBoundary(Boundary boundary) override;

			void onMouseEnter() override;
			void onMouseLeave() override;
			virtual void onMousePress(int button, int mods) override;
			virtual void onMouseRelease(int button, int mods) override;

			Boundary getBoundary() override;
			
		protected:
			bool isHovered;
			bool isPressed;

		private:
			unsigned int texture;
			unsigned int textureHover;
			unsigned int texturePress;
	};

	class IncrementButton : public Button {
		public:
			IncrementButton(unsigned int texture, unsigned int textureHover, unsigned int texturePress);

			void onMouseRelease(int button, int mods) override;

			void setIncrementListener(IncrementListener* listener, double value, double shiftValue, double controlValue, double shiftControlValue);

		private:
			IncrementListener* listener;
			double increaseValue;
			double shiftIncreaseValue;
			double controlIncreaseValue;
			double shiftControlIncreaseValue;
	};

	class TextInput : public Element, public TextLine, public MouseListener, public KeyListener, public FocusListener {
		public:
			TextInput();

			void draw() override;
			void updateBoundary(Boundary boundary) override;
			virtual void onFocus() override;
			virtual void onDefocus() override;
			void onMouseEnter() override;
			void onMouseLeave() override;
			void onMousePress(int button, int mods) override;
			gui::Boundary getBoundary() override;
			virtual void onCharacterPress(char c) override;
			virtual void onKeyPress(int keyCode, int mods, int action) override;
			void setText(std::string text);

			void addCharacter(char c);
			void removeCharacter();
			void removeNextCharacter();

			void setTextOffset(int left, int top);

			void setInsertPosition(int position);
			void setBackgroundImage(unsigned int defaultTexture, unsigned int hoverTexture, unsigned int focusTexture);


		protected:
			bool isFocused;
			bool isHovered;

			void moveInsertPosition(int jumpSize);
		private:
			int insertPosition;
			double previousInsertUpdate;
			int characterLimit;

			int textOffsetLeft;
			int textOffsetTop;

			bool hasBackgroundImage;

			glm::vec4 backgroundColorDefault;
			glm::vec4 backgroundColorHovered;
			glm::vec4 backgroundColorFocused;

			unsigned int backgroundImageDefault;
			unsigned int backgroundImageHovered;
			unsigned int backgroundImageFocused;

			void updateTextPosition();

			void onInsertPositionUpdate();
			bool shouldInsertLineShow();
	};

	class DoubleInput : public TextInput, public IncrementListener {
		public:

			DoubleInput();

			void onCharacterPress(char c) override;
			void onKeyPress(int keyCode, int mods, int action) override;

			void onFocus() override;
			void onDefocus() override;

			void updateValueFromText();
			void updateTextFromValue();
			void increase(double addition) override;
		private:
			double value;
			double maxValue;
			double minValue;

			double getValueWithinBounds(double value);

			static bool isNumber(char c);
			static bool isLetter(char c);
			static std::string doubleToString(double f);
			static double stringToDouble(std::string s);
			static std::string roundStringUp(std::string s);
			static std::string roundString(std::string s);
			static std::string removeLastZeroes(std::string s);
			static bool isCharRoundable(char c);

			static const char roundMinChar = '5';
			static const char roundMaxChar = '9';
	};

	class OGLWindow : public gui::Panel {
		public:
			OGLWindow(int windowWidth, int windowHeight);

			void draw() override;

			bool shoudClose();
			void setMinimalWindowSize(int width, int height);
			void setBackgroundColor(glm::vec4 color);
			void setTitle(std::string title);

			int getWindowWidth();
			int getWindowHeight();

			void updateWindowSize(int width, int height);
			GLFWwindow* glfwWindow;
		private:
			int windowWidth;
			int windowHeight;
	};

	class KeyManager {
		public:
			static void characterCallback(GLFWwindow* window, unsigned int codepoint);
			static void keyCallback(GLFWwindow*, int key, int scanCode, int action, int mods);

			void registerKeyListener(KeyListener* keyListener);
			void onCharacterPress(char c);
			void onKeyPress(int keycode, int mods, int action);
		private:
			std::vector<KeyListener*> keyListeners;
	};

	class MouseManager {
		public:
			static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
			static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
			static void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

			MouseManager();
			void onMouseMove(float posX, float posY);
			void onMouseClick(int button, int action, int mods);
			void onMouseScroll(double horizontalScroll, double verticalScroll);

			float getMouseX();
			float getMouseY();

			void registerMouseListener(MouseListener* listener);
		private:
			float mouseX;
			float mouseY;
			std::vector<MouseListener*> mouseListeners;
	};

	class FocusManager {
		public:
			FocusManager();

			void requestFocus(FocusListener* listener);
		private:
			FocusListener* focusedElementsListener;
	};

	class ResizeManager {
		public:
			static void resizeCallback(GLFWwindow* glfwWindow, int width, int height);
	};

	struct Character { // TODO: Create an actual font class
		unsigned int vao = 0;
		std::vector<Range> ranges;
		float width = 0.0f;

		void draw(glm::mat4 transformationMatrix, glm::vec4 color);
	};

	class Gui {
		/*
			Requires GLFW to be set up
		*/
		public:
			static void init(int windowWidth, int windowHeight);

			static OGLWindow* mainWindow;

			static Shader* shader;
			static unsigned int quadVAO;

			static FocusManager* focusManager;
			static KeyManager* keyManager;
			static MouseManager* mouseManager;
			static ResizeManager* resizeManager;

			static bool isInitialised();
			static glm::mat4 getTransformationMatrix(Boundary boundary);
			static unsigned int loadTexture(std::string pathToFile);
			static Character getCharacter(char c);

		private:
			static std::vector<Character> characters;

			static void loadQuadVAO();
			static void initialiseGLFW();
			static void loadAllCharacters();
			static unsigned int loadCharacterVertices(float vertices[], int size);
			static void setInitialised(bool initialised);
	};
}

struct NotInitializedException : public std::runtime_error {
	NotInitializedException();
};
struct AlreadyInitializedException : public std::runtime_error {
	AlreadyInitializedException();
};
struct WindowCreationException : public std::runtime_error {
	WindowCreationException();
};
struct GladLoadingException : public std::runtime_error {
	GladLoadingException();
};