#include "Framework.h"
#include "DataFormat.h"

#include <Keyboard.h>
#include <psapi.h>
#include "WindowApp.h"
#define VAR_NAME(var) (#var)

Framework::Framework()
{
	frameBuffer_SIZE = { 1920,1080 };
	hwnd = {};
	WINDOW_WIDTH = 1920;
	WINDOW_HEIGHT = 1080;

	FULLSCREEN = FALSE;
}

void Framework::Init(HWND hwnd)
{
    this->hwnd = hwnd;
	HRESULT hr{ S_OK };
	
	CreateFactory();
	
	CreateDevice();
	
	CreateSwapchain();

	ResetViewPort();

	GameEngine::get()->CreateCOM(device.Get(), swapChain.Get(), frameBuffer_SIZE);


	threadActive = false;

	
	//buffer_desc.ByteWidth = ShadowManager::get()->getSizeOfShadowBuffer();
	//hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffers[ConstantBuffer::SHADOWBUFFER].GetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	//
	//buffer_desc.ByteWidth = GraphicEngine::get()->getSizeOfUVScrollBuffer();
	//hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffers[ConstantBuffer::UVSCROLLBUFFER].GetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	//
	//buffer_desc.ByteWidth = GraphicEngine::get()->getSizeOfFogBuffer();
	//hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffers[ConstantBuffer::BLURBUFFER].GetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	//wheel = 0;
}

void Framework::ReleaseSwapchain()
{
	GameEngine::get()->ResetRenderTagertAndDepthStencil();
	if (swapChain)
	{
		swapChain.Get()->SetFullscreenState(FALSE, NULL);
		swapChain.Reset();
	}

}

void Framework::ImGuiSetup()
{
#ifdef USE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, nullptr, glyphRangesJapanese);
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device.Get(), GameEngine::get()->getDeviceContext());
	ImGui::StyleColorsDark();
#endif
}

void Framework::ImGuiUninstall()
{
#ifdef USE_IMGUI
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}

void Framework::ImGuiRender()
{
#ifdef USE_IMGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
#endif
}

void Framework::SetWindowBoderLess()
{
	LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
	lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	SetWindowLong(hwnd, GWL_STYLE, lStyle);
}

void Framework::ImGuiDockSpaceBegin()
{
#ifdef USE_IMGUI
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}


	{
	
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Game", nullptr, window_flags);
		ImGui::End();
		ImGui::Begin("2D Scene", nullptr, window_flags);
		ImGui::End();
		ImGui::Begin("Scene", nullptr, window_flags);
		ImGui::End();
		ImGui::PopStyleVar();
	}
#endif // _DEBUG
}

void Framework::ImGuiDockSpaceEnd()
{
#ifdef USE_IMGUI
	ImGui::End();
#endif // _DEBUG
}


void Framework::ImGuiNewFrame()
{
#ifdef USE_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
#endif
}

void Framework::ImGuiMenuBar()
{
#ifdef USE_IMGUI

	if (ImGui::BeginMenuBar())
	{
		static bool selected = false;
		if (selected)
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			ImGuiStyle* style = &ImGui::GetStyle();
			ImVec4* colors = style->Colors;
			colors[ImGuiCol_WindowBg] = ImVec4(1.06f, 0.06f, 0.06f, 1);
			(ImGui::Begin("Save..", nullptr, window_flags));
			{
				colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
				ImGui::Text("\nYour Scene did not Save at.. Do you want to close this?\n\n");
				ImGui::Indent(120.0f);
				if (ImGui::Button("Yes", { 50,50 }))
				{
					GameEngine::get()->NewScene();
					selected = false;
				}
				ImGui::SameLine();
				ImGui::Indent(130.0f);
				if (ImGui::Button("No", { 50, 50 }))
				{
					selected = false;
				}
				ImGui::Text("\n");
			}
			(ImGui::End());
		}
		if (ImGui::BeginMenu("File", !selected))
		{
			
			if (ImGui::MenuItem("New Scene"))
			{
				if (GameEngine::get()->SceneHadSaveLocal())
				{
					GameEngine::get()->NewScene();
					selected = false;
				}
				else selected = true;

			}
			
			if (ImGui::MenuItem("Open Scene"))
			{

				std::filesystem::path local = GetOpenFileLocal();
				if (local != "")
				{
					if (loadsaveSceneThread.get())
						loadsaveSceneThread->join();
					loadsaveSceneThread.reset();
					threadActive = true;
					loadsaveSceneThread = std::make_unique<std::thread>(&GameEngine::OpenScene, GameEngine::get(), local, &threadActive);
					//GameEngine::get()->OpenScene(local);
				}
					
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Save Scene"))
			{
				bool saveAs = true;
				std::filesystem::path local = "";
				if (GameEngine::get()->SceneHadSaveLocal())
					saveAs = false;
				else local = GetSaveFileLocal();
				
				if (loadsaveSceneThread.get())
					loadsaveSceneThread->join();
				loadsaveSceneThread.reset();
				loadsaveSceneThread = std::make_unique<std::thread>(&GameEngine::SaveScene, GameEngine::get(), local, saveAs);
				//GameEngine::get()->SaveScene(local, saveAs);
			}
			if (ImGui::MenuItem("Save Scene as.."))
			{
				std::filesystem::path local = GetSaveFileLocal();
				if (loadsaveSceneThread.get())
					loadsaveSceneThread->join();
				loadsaveSceneThread.reset();
				loadsaveSceneThread = std::make_unique<std::thread>(&GameEngine::SaveScene, GameEngine::get(), local, true);
				//GameEngine::get()->SaveScene(local, true);
			}
			ImGui::EndMenu();
		}



		

		if (ImGui::BeginMenu("Window", !selected))
		{

			if (ImGui::MenuItem("Exit Game Engine"))
			{
				GameEngine::get()->setExitGame(true);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();

	}

#endif // _DEBUG
}


Framework::~Framework()
{
	ReleaseSwapchain();
	if (loadsaveSceneThread.get())
		loadsaveSceneThread->join();
	loadsaveSceneThread.reset();
	delete GameEngine::get();
}

void Framework::ChangeSizeWindow(UINT64 width, UINT height)
{
	HRESULT hr{ S_OK };
	if (width == 0 || height == 0)
		return;
	if (width != frameBuffer_SIZE.cx || height != frameBuffer_SIZE.cy)
	{
		frameBuffer_SIZE.cx = static_cast<LONG>(width);
		frameBuffer_SIZE.cy = height;
		ResetViewPort();

		CreateSwapchain();

	}

}

void Framework::CalculateFrameStats()
{
	tictoc.tick();
#ifdef USE_IMGUI
	if (++frames, (tictoc.time_stamp() - elapsedTime) >= 1.0f)
	{
		fps = static_cast<float>(frames);
		frames = 0;
		elapsedTime += 1.0f;
		
	}
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	(ImGui::Begin("Game Engine Information", nullptr, window_flags));
	{
		ImGui::Text(GameEngine::get()->getGameModeString().c_str());
		POINT p;
		::GetCursorPos(&p);
		VECTOR2 pos = {(float)p.x,(float)p.y};
		ImGui::Text("Mouse Position: (%.1f,%.1f)", pos.x, pos.y);
		std::string st = "FPS : " + std::to_string(fps) + " / " + "Frame Time : " + std::to_string( 1000.0f / fps ) + " (ms)";
		ImGui::Text(st.c_str());	

		FILETIME idleTime, kernelTime, userTime;
		GetSystemTimes(&idleTime, &kernelTime, &userTime);
		ULARGE_INTEGER kernelTimeValue;
		kernelTimeValue.LowPart = kernelTime.dwLowDateTime;
		kernelTimeValue.HighPart = kernelTime.dwHighDateTime;
		ULARGE_INTEGER userTimeValue;
		userTimeValue.LowPart = userTime.dwLowDateTime;
		userTimeValue.HighPart = userTime.dwHighDateTime;
		ULONGLONG kernelTimeInMs = kernelTimeValue.QuadPart / 10000;
		ULONGLONG userTimeInMs = userTimeValue.QuadPart / 10000;
		std::string str = "CPU Usage: " + std::to_string(kernelTimeInMs + userTimeInMs) + " ms";
		ImGui::Text(str.c_str());


		HANDLE process = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(process, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		std::string str1 = "WorkingSetSize: "+ std::to_string(  pmc.WorkingSetSize / 1000000000.0f).substr(0, std::to_string(  pmc.WorkingSetSize / 1000000000.0f).find('.') + 3) + " Gb";
		std::string str2 = "PrivateUsage: "  + std::to_string(  pmc.PrivateUsage   / 1000000000.0f).substr(0, std::to_string(  pmc.PrivateUsage   / 1000000000.0f).find('.') + 3) + " Gb";
		ImGui::Text(str1.c_str());
		ImGui::Text(str2.c_str());
	}
	ImGui::End();


#endif // !USE_IMGUI
	
}

LRESULT Framework::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef USE_IMGUI
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) { return true; }
#endif
	switch (message)
	{
	case WM_CREATE:
		Init(hWnd);
		break;
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_ACTIVATE:
		if(FULLSCREEN)
		swapChain.Get()->SetFullscreenState(TRUE, NULL);
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ENTERSIZEMOVE:
		tictoc.stop();
		break;
	case WM_MOUSEWHEEL:
		//wheel = GET_WHEEL_DELTA_WPARAM(wparam);
		break;
	case WM_EXITSIZEMOVE:
		tictoc.start();
		break;
	case WM_SIZE:
	{
		RECT client_rect{};
		GetClientRect(hWnd, &client_rect);
		ChangeSizeWindow(static_cast<UINT64>(fabs(client_rect.right) - fabs(client_rect.left)), client_rect.bottom - client_rect.top);
		break;
	}
	case WM_KEYDOWN:
	case WM_KEYUP:
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Framework::AcquireHighPerformanceAdapter(IDXGIFactory6* dxgi_factory6, IDXGIAdapter3** dxgi_adapter3)
{
	HRESULT hr{ S_OK };

	Microsoft::WRL::ComPtr<IDXGIAdapter3> enumerated_adapter;
	for (UINT adapter_index = 0; DXGI_ERROR_NOT_FOUND != dxgi_factory6->EnumAdapterByGpuPreference(adapter_index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(enumerated_adapter.ReleaseAndGetAddressOf())); ++adapter_index)
	{
		DXGI_ADAPTER_DESC1 adapter_desc;
		hr = enumerated_adapter->GetDesc1(&adapter_desc);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		if (adapter_desc.VendorId == 0x1002/*AMD*/ || adapter_desc.VendorId == 0x10DE/*NVIDIA*/)
		{
			OutputDebugStringW((std::wstring(adapter_desc.Description) + L" has been selected.\n").c_str());
			OutputDebugStringA(std::string("\tVendorId:" + std::to_string(adapter_desc.VendorId) + '\n').c_str());
			OutputDebugStringA(std::string("\tDeviceId:" + std::to_string(adapter_desc.DeviceId) + '\n').c_str());
			OutputDebugStringA(std::string("\tSubSysId:" + std::to_string(adapter_desc.SubSysId) + '\n').c_str());
			OutputDebugStringA(std::string("\tRevision:" + std::to_string(adapter_desc.Revision) + '\n').c_str());
			OutputDebugStringA(std::string("\tDedicatedVideoMemory:" + std::to_string(adapter_desc.DedicatedVideoMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tDedicatedSystemMemory:" + std::to_string(adapter_desc.DedicatedSystemMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tSharedSystemMemory:" + std::to_string(adapter_desc.SharedSystemMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tAdapterLuid.HighPart:" + std::to_string(adapter_desc.AdapterLuid.HighPart) + '\n').c_str());
			OutputDebugStringA(std::string("\tAdapterLuid.LowPart:" + std::to_string(adapter_desc.AdapterLuid.LowPart) + '\n').c_str());
			OutputDebugStringA(std::string("\tFlags:" + std::to_string(adapter_desc.Flags) + '\n').c_str());
			break;
		}
	}
	*dxgi_adapter3 = enumerated_adapter.Detach();
}

void Framework::CreateSwapchain()
{
	HRESULT hr{ S_OK };
	if (swapChain)
	{
		GameEngine::get()->ResizeRenderBegin();


		//EffectSeerManager::get()->Deinit();
		DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
		swapChain->GetDesc(&swap_chain_desc);
		hr = swapChain->ResizeBuffers(swap_chain_desc.BufferCount, frameBuffer_SIZE.cx, frameBuffer_SIZE.cy, swap_chain_desc.BufferDesc.Format, swap_chain_desc.Flags);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		GameEngine::get()->ResizeRenderEnd(device.Get(), swapChain.Get(), frameBuffer_SIZE);

		//EffectSeerManager::get()->Init(device.Get(), immediate_context.Get());
	}
	else
	{

		BOOL allow_tearing = FALSE;
		BOOL support = FALSE;
		hr = dxgiFactory6->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(allow_tearing));
		support = SUCCEEDED(hr) && allow_tearing;


		DXGI_SWAP_CHAIN_FULLSCREEN_DESC aa{};
		aa.RefreshRate.Denominator = 1;
		aa.RefreshRate.Numerator = 120;
		aa.Windowed = TRUE;
		aa.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		aa.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;

		DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{};

		swap_chain_desc.Width = frameBuffer_SIZE.cx;
		swap_chain_desc.Height = frameBuffer_SIZE.cy;
		swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.Stereo = FALSE;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = 2;
		swap_chain_desc.Scaling = DXGI_SCALING_NONE;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swap_chain_desc.Flags = support ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		hr = dxgiFactory6->CreateSwapChainForHwnd(device.Get(), hwnd, &swap_chain_desc, &aa, NULL, swapChain.ReleaseAndGetAddressOf());

		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		
	}
}

void Framework::CreateFactory()
{
	HRESULT hr{ S_OK };
	UINT create_factory_flags{ 0 };
#ifdef _DEBUG
	create_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	
	hr = CreateDXGIFactory2(create_factory_flags, IID_PPV_ARGS(dxgiFactory6.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	AcquireHighPerformanceAdapter(dxgiFactory6.Get(), adapter.GetAddressOf());

}

void Framework::CreateDevice()
{
	HRESULT hr{ S_OK };
	UINT create_device_flags{ 0 };
#ifdef _DEBUG
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL feature_levels{ D3D_FEATURE_LEVEL_11_0 };
	hr = D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, 0, create_device_flags, &feature_levels, 1, D3D11_SDK_VERSION, device.ReleaseAndGetAddressOf(),
		NULL, GameEngine::get()->getAddrDeviceContext()
	);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}



void Framework::ResetViewPort()
{
	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(frameBuffer_SIZE.cx);
	viewport.Height = static_cast<float>(frameBuffer_SIZE.cy);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	GameEngine::get()->getDeviceContext()->RSSetViewports(1, &viewport);
}


int Framework::FrameworkRun()
{
	GameEngine::get()->Init(&hwnd);
	
#ifdef USE_IMGUI
	ImGuiSetup();
#endif // USE_IMGUI

	MSG msg;
	GetMessage(&msg, nullptr, 0, 0);
	swapChain.Get()->SetFullscreenState(FULLSCREEN, NULL);
	
	// Main message loop:
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (GameEngine::get()->getExitGame())
				break;
			
#ifdef USE_IMGUI
			ImGuiNewFrame();
			ImGuiDockSpaceBegin();
			ImGui::ShowDemoWindow();
#endif // USE_IMGUI
			CalculateFrameStats();
			if (!threadActive)
			GameRun();
			else
				GameEngine::get()->ClearRenderTagertAndDepthStencil();
#ifdef USE_IMGUI
			ImGuiMenuBar();
			ImGuiDockSpaceEnd();
			ImGuiRender();
#endif // !USE_IMGUI
			PresentFrame();
		}
	}

#ifdef USE_IMGUI
	ImGuiUninstall();
#endif // USE_IMGUI
	

	return 0;
}

void Framework::GameRun()
{
	GameProcessing();
	GameRender();
}

void Framework::GameProcessing()
{
	GameEngine::get()->Update(tictoc.time_interval());
}

void Framework::GameRender()
{
	GameEngine::get()->Render();
}

void Framework::PresentFrame()
{
	//swapChain.Get()->Present(0, DXGI_PRESENT_ALLOW_TEARING);
	swapChain.Get()->Present(1, 0);
}
