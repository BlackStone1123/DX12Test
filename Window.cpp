#include "Window.h"
#include "Graphics.h"

#include "imgui_impl_win32.h"

WindowClass WindowClass::mInstance = WindowClass();
WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, mHInst);
}

WindowClass* WindowClass::GetInstance()
{
	return &mInstance;
}

const WCHAR* WindowClass::GetName() const
{
	return wndClassName;
}

void WindowClass::setup(HINSTANCE hInst)
{
    if (mHInst != hInst)
    {
        mHInst = hInst;

        // Register a window class for creating our render window with.
        WNDCLASSEXW wc = {};

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = &Window::HandleMsgSetup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInst;
        wc.hIcon = ::LoadIcon(hInst, NULL);
        wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = GetName();
        wc.hIconSm = ::LoadIcon(hInst, NULL);

        RegisterClassEx(&wc);
    }
}

Window::Window(HINSTANCE instance, int width, int height, const WCHAR* name)
	: mWidth(width), mHeight(height)
{
    WindowClass* pWndClass = WindowClass::GetInstance();
    if (pWndClass)
    {
        pWndClass->setup(instance);
        createWindow(pWndClass->GetName(), instance, name, width, height);
        ImGui_ImplWin32_Init(mHwnd);
        mGfx = std::make_unique<Graphics>(mHwnd, width, height);
        ShowWindow(mHwnd, SW_SHOWDEFAULT);
    }
    else
    {
        throw std::exception();
    }
}

Graphics* Window::Gfx()
{
    if (!mGfx)
    {
        throw std::exception();
    }
    return mGfx.get();
}

void Window::createWindow(
    const WCHAR* windowClassName, HINSTANCE hInst,
    const WCHAR* windowTitle, uint32_t width, uint32_t height
)
{
    int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

    RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    // Center the window within the screen. Clamp to 0, 0 for the top-left corner.
    int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
    int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);

    mHwnd = ::CreateWindow(
        windowClassName,
        windowTitle,
        WS_OVERLAPPEDWINDOW,
        windowX, windowY,
        windowWidth, windowHeight,
        nullptr, nullptr,
        hInst,
        this
    );

    // check for error
    if (mHwnd == nullptr)
    {
        throw std::exception();
    }
}

Window::~Window()
{
    DestroyWindow(mHwnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window instance handler
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // retrieve ptr to window instance
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window instance handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

	switch (msg)
	{
		// we don't want the DefProc to handle this message because
		// we want our destructor to destroy the window, so return 0 instead of break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
    case WM_SIZE:
    {
        RECT clientRect = {};
        ::GetClientRect(mHwnd, &clientRect);

        int width = clientRect.right - clientRect.left;
        int height = clientRect.bottom - clientRect.top;

        mGfx->Resize(width, height);
    }break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

std::optional<int> Window::ProcessMessages()
{
    MSG msg;
    // while queue has messages, remove and dispatch them (but do not block on empty queue)
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        // check for quit because peekmessage does not signal this via return val
        if (msg.message == WM_QUIT)
        {
            // return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
            return (int)msg.wParam;
        }

        // TranslateMessage will post auxilliary WM_CHAR messages from key msgs
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // return empty optional when not quitting app
    return {};
}
