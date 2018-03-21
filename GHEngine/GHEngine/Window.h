#pragma once
#include <cstdint>
#include <glfw/glfw3.h>


namespace GHGraphics
{
	class Window
	{
	public:
		Window();
		~Window();

		int32_t Init();

		GLFWwindow* window;
		uint32_t height = 600, width = 800;
	};
}
