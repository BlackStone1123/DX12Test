#include "Helpers.h"
#include <shellapi.h> // For CommandLineToArgvW
#include "App.h"

void ParseCommandLineArguments(int& clientWidth, int& clientHeight)
{
    int argc;
    wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

    for (size_t i = 0; i < (size_t)argc; ++i)
    {
        if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0)
        {
            clientWidth = ::wcstol(argv[++i], nullptr, 10);
        }
        if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0)
        {
            clientHeight = ::wcstol(argv[++i], nullptr, 10);
        }
    }

    // Free memory allocated by CommandLineToArgvW
    ::LocalFree(argv);
}

//void Update()
//{
//    static uint64_t frameCounter = 0;
//    static double elapsedSeconds = 0.0;
//    static std::chrono::high_resolution_clock clock;
//    static auto t0 = clock.now();
//
//    frameCounter++;
//    auto t1 = clock.now();
//    auto deltaTime = t1 - t0;
//    t0 = t1;
//    elapsedSeconds += deltaTime.count() * 1e-9;
//    if (elapsedSeconds > 1.0)
//    {
//        std::cout << "FPS: " << frameCounter / elapsedSeconds << std::endl;
//
//        frameCounter = 0;
//        elapsedSeconds = 0.0;
//    }
//}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    int clientWidth = 1200;
    int clientHeight = 800;
    ParseCommandLineArguments(clientWidth, clientHeight);

    try
    {
        return App(hInstance, clientWidth, clientHeight).exec();
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, L"·¢ÏÖÒì³£", L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}