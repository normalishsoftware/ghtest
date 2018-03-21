#include "TestGame.h"


TestGame::TestGame()
{
}

TestGame::~TestGame()
{
}

void TestGame::Run()
{
	InitSystems();
}

void TestGame::InitSystems()
{
	m_window.Init();
	m_graphicsManager.InitVulkan();
	Update();
}

void TestGame::Update()
{
	while (!glfwWindowShouldClose(m_window.window))
	{
		glfwPollEvents();
	}
}
