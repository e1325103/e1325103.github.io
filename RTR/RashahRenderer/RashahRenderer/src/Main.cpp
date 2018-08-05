#include <iostream>

#include "Window.h"


void handleKeyEvent(KEY k, KEY_ACTION a);

auto window = Window();

int main()
{

	bool res = window.init();
	if (!res) {
		std::cout << "ERROR: Init Window :(" << std::endl;
		return EXIT_FAILURE;
	}
	window.setKeyChangedCallback(handleKeyEvent);
	res = window.open(800, 800, false, "The Lost Treasure of Alman-Rashah");

	while (!window.shouldClose())
	{
		glfwPollEvents();
	}

	if (!res) {
		std::cout << "ERROR: Open Window :(" << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}

void handleKeyEvent(KEY k, KEY_ACTION a) {
	switch (k)
	{
	case KEY::ESC:
		std::cout << "ESC" << std::endl;
		window.close();
		break;
	default:
		break;
	}
}