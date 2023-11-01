// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <sstream>

#include <dxgi1_6.h>
#include <wrl.h>

#include "Misc.h"
#include "HighResolutionTimer.h"

#include "GameEngine.h"


#ifdef USE_IMGUI
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#include "UsesImGui_h.h"
extern ImWchar glyphRangesJapanese[];
#endif




class Framework : public SingletonPatternTemplate<Framework>
{
	friend class SingletonPatternTemplate<Framework>;
public:
	Framework();
	void Init(HWND hwnd);
	int FrameworkRun();
	void GameRun();
	void GameProcessing();
	void GameRender();
	void PresentFrame();
	const HWND* getHwnd() { return &hwnd; }
	~Framework();

public:
	CONST LONG& getScreenWidth() { return WINDOW_WIDTH; }
	CONST LONG& getScreenHeight() { return WINDOW_HEIGHT; }
	CONST BOOL& getFullScreen() { return FULLSCREEN; }
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	ID3D11Device* getDevice() { return device.Get(); }
	IDXGISwapChain1* getSwapChain() { return swapChain.Get(); }
	
	float getElapsedTime() { return elapsedTime; }
private:
	HWND hwnd;
	SIZE frameBuffer_SIZE;
	LONG WINDOW_WIDTH{ 1920 };
	LONG WINDOW_HEIGHT{ 1080 };

	BOOL FULLSCREEN{ FALSE };
	bool threadActive;
	std::unique_ptr<std::thread> loadsaveSceneThread;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;

	Microsoft::WRL::ComPtr<IDXGIAdapter3> adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory6;


private:
	void AcquireHighPerformanceAdapter(IDXGIFactory6* dxgi_factory6, IDXGIAdapter3** dxgi_adapter3);
	void CreateSwapchain();
	void CreateFactory();
	void CreateDevice();

	void ChangeSizeWindow(UINT64 width, UINT height);
	void CalculateFrameStats();
	void ResetViewPort();
	void ReleaseSwapchain();
	void ImGuiSetup();
	void ImGuiUninstall();
	void ImGuiRender();
	void SetWindowBoderLess();

	void ImGuiDockSpaceBegin();
	void ImGuiDockSpaceEnd();
	void ImGuiNewFrame();
	void ImGuiMenuBar();
private:
	HighResolutionTimer tictoc;
	uint32_t frames{ 0 };
	float elapsedTime{ 0.0f };
	float fps;



private:
};

