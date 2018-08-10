#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

enum class KEY {
	NONE, ESC
};

enum class KEY_ACTION {
	DOWN, UP
};

typedef void(*KeyCallback)(KEY key, KEY_ACTION action);

void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods);

static KeyCallback _keyCB;

class Window {
public:
	Window();
	~Window();

	bool init();
	bool open(int width, int height, bool fullscreen, const char* title);

	void setKeyChangedCallback(KeyCallback keyCB);

	bool shouldClose();
	void close();

	void refresh();
private:
	int _width = 0;
	int _height = 0;
	bool _shouldClose = false;
	GLFWwindow* _window;
};