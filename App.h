#pragma once

#include "Helpers.h"
#include "Timer.h"

class Window;
class ImguiManager;
class Drawable;
class App
{
public:
	App(HINSTANCE hIns, int width, int height);
	~App();
	int exec();

private:
	std::unique_ptr<ImguiManager> mImguiManager;
	std::unique_ptr<Window> mWnd;

	std::vector<std::unique_ptr<Drawable>> mDrawables;

	ChiliTimer mTimer;
	float mSpeedFactor = 1.0f;
};