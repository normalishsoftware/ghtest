#include "Window.h"


namespace GHGraphics
{
	Window::Window()
	{
	}

	Window::~Window()
	{
	}

	int32_t Window::Init()
	{
		if (!glfwInit())
		{
			return -1;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, "GHEngine", nullptr, nullptr);

		return 0;
	}
}
