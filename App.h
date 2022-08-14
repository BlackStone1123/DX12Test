#pragma once

#include "Helpers.h"

class Window;
class ImguiManager;
class Box;

class App
{
public:
	App(HINSTANCE hIns, int width, int height);
	~App();

	int exec();
private:
	std::unique_ptr<ImguiManager> mImguiManager;
	std::unique_ptr<Window> mWnd;

	std::vector<std::unique_ptr<Box>> boxes;
	bool mShowDemoWindow{ true };
};