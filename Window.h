#pragma once

#include "Helpers.h"
#include <optional>

class Graphics;

class WindowClass
{
public:
	static WindowClass* GetInstance();
	const WCHAR* GetName() const;
	void setup(HINSTANCE hInst);

private:
	WindowClass() = default;
	~WindowClass();
	WindowClass(const WindowClass&) = delete;
	WindowClass& operator=(const WindowClass&) = delete;

private:
	const WCHAR* wndClassName = L"DX12TestingWindowClass";
	HINSTANCE mHInst;
	static WindowClass mInstance;
};

class Window
{
	friend class WindowClass;
public:
	Window(HINSTANCE hInst, int width, int height, const WCHAR* windowTitle);
	~Window();

	std::optional<int> ProcessMessages();
	Graphics* Gfx();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void createWindow(
		const WCHAR* windowClassName, HINSTANCE hInst,
		const WCHAR* windowTitle, uint32_t width, uint32_t height
	);

private:
	int mWidth{ 500 };
	int mHeight{ 500 };
	HWND mHwnd;
	std::unique_ptr<Graphics> mGfx;
};