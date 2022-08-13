#pragma once

#include "Helpers.h"

class Window;
class Box;
class App
{
public:
	App(HINSTANCE hIns, int width, int height);
	~App();

	int exec();
private:
	std::unique_ptr<Window> mWnd;
	std::vector<std::unique_ptr<Box>> boxes;
};