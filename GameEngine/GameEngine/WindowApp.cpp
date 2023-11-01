// GraphicEngine.cpp : Defines the entry point for the application.
//

#include "resource.h"
#include "Framework.h"
#include "WindowApp.h"
#include <Windows.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100

// Global Variables:
//HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hWnd{};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
   
#ifdef _DEBUG
    LoadStringW(hInstance, IDS_APP_TITLE_DEBUG, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEENGINE_DEBUG, szWindowClass, MAX_LOADSTRING);
#else
    LoadStringW(hInstance, IDS_APP_TITLE_RELEASE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEENGINE_RELEASE, szWindowClass, MAX_LOADSTRING);
#endif // _DEBUG
    
    FLOAT dpiX, dpiY;
    MyRegisterClass(hInstance);
    SetProcessDPIAware();
    HDC screen = GetDC(0);
    dpiX = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSX));
    dpiY = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSY));
    ReleaseDC(0, screen);
    int width = Framework::get()->getScreenWidth(); (int)(dpiX * Framework::get()->getScreenWidth() / 96);
    int height = Framework::get()->getScreenHeight(); (int)(dpiY * Framework::get()->getScreenHeight() / 96);
    //UINT dpi = GetDpiForSystem();

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE/*^ WS_MAXIMIZEBOX ^ WS_THICKFRAME*/ | WS_VISIBLE,
        0, 0, width, height, nullptr, nullptr, hInstance, nullptr);
    //SetWindowPos(hWnd, NULL, 0, 0, (int)(dpiX * framework.getScreenWidth() / 96), (int)(dpiY * framework.getScreenHeight() / 96), SWP_NOZORDER | SWP_NOMOVE);
    
    HACCEL hAccelTable;
#ifdef _DEBUG
    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEENGINE_DEBUG));
#else
    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEENGINE_RELEASE));
#endif // _DEBUG

    int q = Framework::get()->FrameworkRun();;
    delete Framework::get();
    return q;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    LPCWSTR name;
#ifdef _DEBUG
    name = MAKEINTRESOURCEW(IDC_GAMEENGINE_DEBUG);
#else
    name = MAKEINTRESOURCEW(IDC_GAMEENGINE_RELEASE);
#endif // _DEBUG
    wcex.lpszMenuName   = name;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    return Framework::get()->WndProc(hWnd, message, wParam, lParam);
}

std::string GetOpenFileLocal()
{
    OPENFILENAMEA ofName = { 0 };
    CHAR fileName[260] = { 0 };
    ZeroMemory(&fileName, sizeof(OPENFILENAME));
    ofName.lStructSize = sizeof(OPENFILENAME);
    ofName.hwndOwner = hWnd;
    ofName.lpstrFile = fileName;
    ofName.nMaxFile = sizeof(fileName);
    ofName.lpstrFilter = "Data (*.sce)\0*.sce\0";
    ofName.nFilterIndex = 1;
    ofName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetOpenFileNameA(&ofName) == TRUE)
    {
        return ofName.lpstrFile;
    }
    return "";
}

std::string GetSaveFileLocal()
{
    OPENFILENAMEA ofName = { 0 };
    CHAR fileName[260] = { 0 };
    ZeroMemory(&fileName, sizeof(OPENFILENAME));
    ofName.lStructSize = sizeof(OPENFILENAME);
    ofName.hwndOwner = hWnd;
    ofName.lpstrFile = fileName;
    ofName.nMaxFile = sizeof(fileName);
    ofName.lpstrFilter = "Data (*.sce)\0*.sce\0";
    ofName.nFilterIndex = 1;
    ofName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (GetSaveFileNameA(&ofName) == TRUE)
    {
        return ofName.lpstrFile;
    }
    return "";
}
