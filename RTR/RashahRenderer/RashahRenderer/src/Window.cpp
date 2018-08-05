#include "Window.h"

Window::Window() : _width(0), _height(0) {

}

Window::~Window() {
	glfwTerminate();
}

bool Window::init() {
	if (!glfwInit()) {
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	return true;
}

bool Window::open(int width, int height, bool fullscreen, const char* title) {
	_width = width;
	_height = height;

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWmonitor* monitor = nullptr;

	if (fullscreen) {
		monitor = glfwGetPrimaryMonitor();
	}

	_window = glfwCreateWindow(width, height, title, monitor, nullptr);
	if (!_window) {
		return false;
	}

	glfwMakeContextCurrent(_window);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		return false;
	}
	glewExperimental = true;

	glfwSetKeyCallback(_window, &keyChanged);

	return true;
}

bool Window::shouldClose() {
	return _shouldClose;
}

void Window::close() {
	_shouldClose = true;
}

void Window::setKeyChangedCallback(KeyCallback keyCB) {
	_keyCB = keyCB;
}

void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods) {

	KEY_ACTION a;
	KEY k;

	switch (action)
	{
	case GLFW_PRESS:
		a = KEY_ACTION::DOWN;
		break;
	case GLFW_RELEASE:
		a = KEY_ACTION::UP;
		break;
	default:
		a = KEY_ACTION::UP;
		break;
	}

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		k = KEY::ESC;
		break;
	default:
		k = KEY::NONE;
		break;
	}

	_keyCB(k, a);
}