#pragma once
#include <GHEngine\Window.h>
#include <GHEngine\GraphicsManager.h>


class TestGame
{
public:
	TestGame();
	~TestGame();

	void Run();
	void InitSystems();
	void Update();

	GHGraphics::Window m_window;
	GHGraphics::GraphicsManager m_graphicsManager;
};
