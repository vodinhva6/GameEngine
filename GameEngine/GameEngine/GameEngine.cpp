#include "GameEngine.h"
#include "GraphicEngine.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "ActorManager.h"
#include "ContentBrowser.h"
#include "Framework.h"

#ifdef USE_IMGUI
#include "Log.h"
static ApplicationConsole console;
std::unique_ptr<ContentBrowser> contentBrowserTree;
#endif // USE_IMGUI

GameEngine::GameEngine()
{
#ifdef USE_IMGUI
	contentBrowserTree = std::make_unique<ContentBrowser>();
#endif // USE_IMGUI
	runMode = EngineRunMode::EngineDebugMode;
	exitGame = false;
}

void GameEngine::Init(const HWND* hwnd)
{
	GraphicEngine::get()->Init();
	InputEngine::get()->Init(hwnd);
	MyAudioEngine::get()->Init();
	//actorManager.get()->Init();
#ifdef USE_IMGUI
	contentBrowserTree.get()->Init("Data\\Asset");
#endif // USE_IMGUI
	//OpenScene("Data\\Asset\\Scene\\mainScene.sce",nullptr);
}

void GameEngine::Update(float elapsedTime)
{
	InputEngine::get()->Update();
#ifdef USE_IMGUI
	contentBrowserTree.get()->Update();
	CheckChangeRunGameMode();
#endif // _DEBUG
	GraphicEngine::get()->Update(elapsedTime);

}

void GameEngine::Render()
{
	GraphicEngine::get()->Render();
	bool open = !exitGame;
#ifdef USE_IMGUI
	console.Draw("Console", &(open));
	contentBrowserTree.get()->DrawOn();
	
#endif // USE_IMGUI
	GraphicEngine::get()->RemoveQueneObject();
}

void GameEngine::CreateCOM(ID3D11Device* device, IDXGISwapChain1* swapChain, const SIZE& frameSize)
{
	GraphicEngine::get()->CreateCOM(device, swapChain, frameSize);
}

void GameEngine::ResizeRenderBegin()
{
	ID3D11RenderTargetView* null_render_target_view{};
	GraphicEngine::get()->getDeviceContext()->OMSetRenderTargets(1, &null_render_target_view, NULL);
	GraphicEngine::get()->ResetRenderTagertAndDepthStencil();
	GraphicEngine::get()->ResetFrameBufferManager();
}

void GameEngine::ResizeRenderEnd(ID3D11Device* device, IDXGISwapChain1* swapChain, const SIZE& frameSize)
{
	GraphicEngine::get()->CreateRenderTagert(device, swapChain);
	GraphicEngine::get()->CreateDepthStencilView(device, swapChain, frameSize);
	D3D11_VIEWPORT viewPort;
	UINT viewNum = 1;
	GraphicEngine::get()->getDeviceContext()->RSGetViewports(&viewNum, &viewPort);
	GraphicEngine::get()->CreateFramebuffer(device, swapChain);
}

void GameEngine::DrawAllUninvisibleObject(FrameBufferName name, DrawType type)
{
	GraphicEngine::get()->DrawAllUninvisibleObject((int)name, type);
}

void GameEngine::CastShadow(FrameBufferName name, DrawType type, UINT numIndex)
{
	GraphicEngine::get()->CastShadow((int)name, type, numIndex);
}

void GameEngine::ResetRenderTagertAndDepthStencil()
{
	GraphicEngine::get()->ResetRenderTagertAndDepthStencil();
}

void GameEngine::NewScene()
{
	GraphicEngine::get()->NewScene();
}

void GameEngine::SaveScene(std::filesystem::path local, bool saveAs)
{
	GraphicEngine::get()->SaveScene(local, saveAs);
}

void GameEngine::OpenScene(std::filesystem::path local, bool* result)
{
	GraphicEngine::get()->OpenScene(local);
	if(result)
		*result = false;
}

bool GameEngine::SceneHadSaveLocal()
{
	return GraphicEngine::get()->SceneHadSaveLocal();
}

void GameEngine::ClearRenderTagertAndDepthStencil()
{
	GraphicEngine::get()->ClearRenderTagertAndDepthStencil();
}

void GameEngine::SetContentDirectory(std::filesystem::path directory)
{
#ifdef USE_IMGUI
	contentBrowserTree->SetItemSelect(directory);
#endif // USE_IMGUI
}

void GameEngine::DrawSkymap(bool debug)
{
	GraphicEngine::get()->DrawSkymap(debug);
}

void GameEngine::SetIBLSkyMap()
{
	GraphicEngine::get()->SetIBLSkyMap();
}

GameEngine::~GameEngine()
{
#ifdef USE_IMGUI
	contentBrowserTree.reset();
#endif // USE_IMGUI
	//actorManager.reset();
	delete GraphicEngine::get();
	delete InputEngine::get();
	delete MyAudioEngine::get();
}

void GameEngine::CreateVS(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader, ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements)
{

	HRESULT hr = GraphicEngine::get()->getShaderManager()->createVSFromFile_cso(device, cso_name, vertex_shader, input_layout, input_element_desc, num_elements);
#ifdef _DEBUG
	SUCCEEDEDRESULT(hr);
#endif // _DEBUG

}

void GameEngine::CreatePS(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader)
{
	HRESULT hr = GraphicEngine::get()->getShaderManager()->createPSFromFile_cso(device, cso_name, pixel_shader);
#ifdef _DEBUG
	SUCCEEDEDRESULT(hr);
#endif // _DEBUG
}


bool GameEngine::CreateRay(pSmartVoid& ray, const VECTOR3& begin, const VECTOR3& end, const VECTOR4& color, int rayType)
{
	//ray.reset(GraphicEngine::get()->createRay(begin, end, color, (RayType)(rayType)));
	return true;
}

bool GameEngine::LoadSkinnedMesh(std::filesystem::path fileLocal, pSmartVoid& mesh)
{
	return GraphicEngine::get()->CreateSkinnedMesh(fileLocal, mesh);
}

bool GameEngine::CreateSkinnedMeshThumbnail(std::filesystem::path fileName, pSmartVoid* mesh, bool* result)
{
	bool lastReturn = LoadSkinnedMesh(fileName.string().c_str(), *mesh);
	if (result)
		*result = true;
	return lastReturn;
}

bool GameEngine::LoadMaterial(std::filesystem::path fileName, pSmartVoid& material )
{
	return GraphicEngine::get()->LoadMaterial(fileName, material);
}

bool GameEngine::LoadAnimator(std::filesystem::path fileName, pSmartVoid& animator)
{
	return GraphicEngine::get()->LoadAnimator(fileName, animator);
}

bool GameEngine::LoadAnimation(std::filesystem::path fileName, pSmartVoid& animation)
{
	return GraphicEngine::get()->LoadAnimation(fileName, animation);
}

bool GameEngine::LoadMaterialThumbnail(std::filesystem::path fileName, pSmartVoid* material, bool* result)
{
	bool createResult = LoadMaterial(fileName, *material);
	if (result)
		*result = true;
	return createResult;
}

void GameEngine::LoadAnimatorThumbnail(std::filesystem::path fileName, pSmartVoid* Animator, bool* result)
{
	*result = GraphicEngine::get()->LoadAnimator(fileName, *Animator);
}

void GameEngine::LoadAnimationThumbnail(std::filesystem::path fileName, pSmartVoid* Animation, bool* result)
{
	*result = GraphicEngine::get()->LoadAnimation(fileName, *Animation);
}

bool GameEngine::CreateCapsuleDebug(void* cap, const float& height, const float& weight, const VECTOR3& offset)
{
<<<<<<< HEAD
	//BoundingCapsule* pCap = std::static_pointer_cast<BoundingCapsule>(cap).get();
	//pCap = GraphicEngine::get()->createCapsuleDebug(height, weight, offset);
	//cap.reset(pCap);
=======
	BoundingCapsule* pCap = static_cast<BoundingCapsule*>(cap);
	pCap = GraphicEngine::get()->createCapsuleDebug(height, weight, offset);
>>>>>>> parent of a31de18 (dda)
	return true;
}

bool GameEngine::createSphereDebug(void* sphere, const float& radius, const VECTOR3& offset)
{
<<<<<<< HEAD
	//BoundingSphere* pSphere = std::static_pointer_cast<BoundingSphere>(sphere).get();
	//pSphere = GraphicEngine::get()->createSphereDebug(radius, offset);
=======
	BoundingSphere* pSphere = static_cast<BoundingSphere*>(sphere);
	pSphere = GraphicEngine::get()->createSphereDebug(radius, offset);
>>>>>>> parent of a31de18 (dda)
	return true;
}

bool GameEngine::CreateSprite3D(std::filesystem::path local, pSmartVoid& sprite3D)
{
	return 	GraphicEngine::get()->CreateSprite3D(local, sprite3D);
}

void GameEngine::SpriteTextOutScreen(int textNo, const std::string contents, const VECTOR2& position,
	const float& size, const VECTOR4& color, const int& drawTurn, bool late)
{
	GraphicEngine::get()->TextOut(textNo, contents, position, size, color, drawTurn, late);
}

void GameEngine::DrawSkinnedMesh(pSmartVoid mesh, const DirectX::XMFLOAT4X4 world,
	pVoid materialList, const VECTOR4& color, const int& drawTurn,
	DrawStates& drawStates, pVoid keyFrame, FrameBufferName name)
{
	SkinnedMesh* rMesh = GetFrom<SkinnedMesh>(mesh);
	std::unordered_map<int64_t, std::shared_ptr<Material>>* pMaterialList = GetFrom<std::unordered_map<int64_t, std::shared_ptr<Material>>>(materialList);
	Animation::Keyframe* pKeyFrame = GetFrom<Animation::Keyframe>(keyFrame);
	GraphicEngine::get()->DrawSkinnedMesh(rMesh, world, pMaterialList, color, drawTurn, drawStates, pKeyFrame, name);
}

void GameEngine::DrawSpriteLate(void* sprite, const VECTOR2& position, const VECTOR2& scale, const VECTOR4& color,
	const VECTOR2& texture_position, const VECTOR2& texture_size, const float& angle, const int& drawTurn)
{
	Sprite* pSprite = static_cast<Sprite*>(sprite);
	GraphicEngine::get()->DrawSpriteLate(pSprite, position, scale, color, texture_position, texture_size, angle, drawTurn);
}

void GameEngine::DrawSprite3D(pSmartVoid sprite3D, const DirectX::XMFLOAT4X4 world, const VECTOR4& color, int drawTurn, FrameBufferName name)
{
	Sprite3D* pSprite3D = GetFrom<Sprite3D>(sprite3D);
	GraphicEngine::get()->DrawSprite3D(pSprite3D, world, color, drawTurn, name);
}

void GameEngine::DrawDebugBounding(void* geo, const DirectX::XMFLOAT4X4 world, const VECTOR4& color, const int& type)
{
<<<<<<< HEAD
	//GeometricPrimitive* pGeo = std::static_pointer_cast<GeometricPrimitive>(geo).get();
	//GraphicEngine::get()->drawDebugBounding(pGeo, world, color, type);
=======
	GeometricPrimitive* pGeo = static_cast<GeometricPrimitive*>(geo);
	GraphicEngine::get()->drawDebugBounding(pGeo, world, color, type);
>>>>>>> parent of a31de18 (dda)
}

void GameEngine::LoadTextureFromFileMultiThread(std::wstring fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* shader_resource_view, bool* result)
{
	LoadTextureFromFile(fileName, *shader_resource_view, result);

	if (result)
		*result = true;
}

bool GameEngine::LoadTextureFromFile(std::wstring fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, bool* result)
{
	return GraphicEngine::get()->LoadTextureFromFile(fileName, shader_resource_view, result);
}

bool GameEngine::LoadTextureFromFile(std::filesystem::path local, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, bool* result)
{
	return GraphicEngine::get()->LoadTextureFromFile(local.wstring().c_str(), shader_resource_view, result);
}

bool GameEngine::LoadTextureFromFile(std::filesystem::path local, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc)
{
	return GraphicEngine::get()->getTextureManager()->loadTextureFromFile(getDevice(), local, shader_resource_view, texture2d_desc);
}

bool GameEngine::LoadTextureFromMemory(const void* data, size_t size, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView)
{
	return GraphicEngine::get()->getTextureManager()->LoadTextureFromMemory(getDevice(), data, size, shaderResourceView);
}


bool GameEngine::ReleaseTexture(std::filesystem::path dir)
{
	return GraphicEngine::get()->getTextureManager()->ReleaseTexture(dir);
}

bool GameEngine::ReleaseMesh(std::filesystem::path local)
{
	return GraphicEngine::get()->getMeshLoaderManager()->ReleaseMesh(local);
}

bool GameEngine::ReleaseMaterial(std::filesystem::path local)
{
	return GraphicEngine::get()->getMaterialManager()->ReleaseMaterial(local);
}

bool GameEngine::ReleaseAnimator(std::filesystem::path local)
{
	return GraphicEngine::get()->getAnimatorManager()->ReleaseAnimator(local);;
}

bool GameEngine::ReleaseAnimation(std::filesystem::path local)
{
	return GraphicEngine::get()->getAnimatorManager()->ReleaseAnimation(local);;
}

bool GameEngine::RebuildMaterial(pSmartVoid material)
{
	std::shared_ptr<Material> pMaterial = GetFromPoint<Material>(material);
	GraphicEngine::get()->getMaterialManager()->BuildNewMaterialCereal(pMaterial);
	GraphicEngine::get()->getMaterialManager()->LoadTextureShaderResource(pMaterial);
	return true;
}

bool GameEngine::RenameMaterial(std::filesystem::path newName, std::filesystem::path oldName)
{
	return GraphicEngine::get()->getMaterialManager()->RenameMaterial(oldName, newName);
}

bool GameEngine::RenameAnimator(std::filesystem::path newName, std::filesystem::path oldName)
{
	return GraphicEngine::get()->getAnimatorManager()->RenameAnimator(oldName, newName);
}

bool GameEngine::RenameAnimation(std::filesystem::path newName, std::filesystem::path oldName)
{
	return GraphicEngine::get()->getAnimatorManager()->RenameAnimation(oldName, newName);;
}

bool GameEngine::DeleteMaterial(std::filesystem::path local)
{
	return GraphicEngine::get()->getMaterialManager()->DeleteMaterial(local);
}

bool GameEngine::DeleteAnimator(std::filesystem::path local)
{
	return GraphicEngine::get()->getAnimatorManager()->DeleteFileAnimator(local);
}

bool GameEngine::SetMeshDefaultTransform(std::filesystem::path fileLocal, const VECTOR3& posOff, const VECTOR3& rotOff, const VECTOR3& scaOff)
{
	return GraphicEngine::get()->getMeshLoaderManager()->SetDefaultTransform(fileLocal, posOff, rotOff, scaOff);
}

std::filesystem::path GameEngine::CreateNewDummyMaterial(std::filesystem::path path)
{
	return GraphicEngine::get()->getMaterialManager()->CreateNewDummyMaterial(path)->getFileLocal();
}

std::filesystem::path GameEngine::CreateNewAnimator(std::filesystem::path path)
{
	return GraphicEngine::get()->getAnimatorManager()->CreateNewAnimator(path)->localfile;
}


float GameEngine::getAspectRatio(FrameBufferName name)
{
	return GraphicEngine::get()->getAspectRatio(name);
}

std::string GameEngine::DropData(std::string dataType, int flag)
{
#ifdef USE_IMGUI
	return contentBrowserTree->DropData(dataType,(ImGuiDragDropFlags)flag);
#else
	return "";
#endif // USE_IMGUI
}

pVoid GameEngine::getMousePoint()
{
	return SetTo<MousePoint>(InputEngine::get()->getMousePoint());
}

pVoid GameEngine::getControlPad()
{
	return SetTo<ControlPad>(InputEngine::get()->getControlPad());
}

ID3D11Device* GameEngine::getDevice()
{
	return Framework::get()->getDevice();
}

IDXGISwapChain* GameEngine::getSwapChain()
{
	return Framework::get()->getSwapChain();
}

ID3D11DeviceContext* GameEngine::getDeviceContext()
{
	return GraphicEngine::get()->getDeviceContext();
}

ID3D11DeviceContext** GameEngine::getAddrDeviceContext()
{
	return GraphicEngine::get()->getAddrDeviceContext();;
}

D3D11_VIEWPORT& GameEngine::getViewPort(FrameBufferName name)
{
	return GraphicEngine::get()->getViewPort(name);
}

float GameEngine::getElapsedTime()
{ 
	return Framework::get()->getElapsedTime();
}



pVoid GameEngine::getActorManager()
{
	return SetTo<ActorManager>(GraphicEngine::get()->getSceneManager()->getActorManager());
}

pVoid GameEngine::getFrameBufferManager()
{
	return SetTo(GraphicEngine::get()->getFrameBufferManager());
}


pVoid GameEngine::getCameraManager()
{
	return SetTo(GraphicEngine::get()->getSceneManager()->getCameraManager());
}

pVoid GameEngine::getShaderManager()
{
	return SetTo(GraphicEngine::get()->getShaderManager());
}

pVoid GameEngine::getLightManager()
{
	return SetTo(GraphicEngine::get()->getSceneManager()->getSceneNow()->getLightManager());
}

pVoid GameEngine::getDepthStencilStateManager()
{
	return SetTo(GraphicEngine::get()->getDepthStencilStateManager());
}

pVoid GameEngine::getSamplerStateManager()
{
	return SetTo(GraphicEngine::get()->getSamplerStateManager());
}

pVoid GameEngine::getBlendStateManager()
{
	return SetTo(GraphicEngine::get()->getBlendStateManager());
}

pVoid GameEngine::getRasterizerStateManager()
{
	return SetTo(GraphicEngine::get()->getRasterizerStateManager());
}

pVoid GameEngine::getTextureManager()
{
	return SetTo(GraphicEngine::get()->getTextureManager());
}

pVoid GameEngine::getMaterialManager()
{
	return SetTo(GraphicEngine::get()->getMaterialManager());
}

pVoid GameEngine::getMeshLoaderManager()
{
	return SetTo(GraphicEngine::get()->getMeshLoaderManager());
}

pVoid GameEngine::getContentBrowser()
{
#ifdef USE_IMGUI
	return SetTo(contentBrowserTree.get());
#else
	return nullptr;
#endif // USE_IMGUI
}

pVoid GameEngine::getAnimatorManager()
{
	return SetTo(GraphicEngine::get()->getAnimatorManager());
}

void GameEngine::CheckChangeRunGameMode()
{
	ControlPad* controlPad = InputEngine::get()->getControlPad();
	if (controlPad->PressF1(0))
	{
		if (runMode == EngineRunMode::EngineDebugMode)
		{
			runMode = EngineRunMode::EngineGameMode;
			while (ShowCursor(false) >= 0) {}
			InputEngine::get()->getControlPad()->SetControllerType(Controler::GAMEPAD);
			std::shared_ptr<Camera> gameCamera = 
				GraphicEngine::get()->getSceneManager()->getCameraManager()->getCamera(CameraName::GameScene);
			gameCamera->setFPSLock(true);
		}
		else
		{
			runMode = EngineRunMode::EngineDebugMode;
			while (ShowCursor(true) < 0) {}
			InputEngine::get()->getControlPad()->SetControllerType(Controler::KEYBOARD);
			std::shared_ptr<Camera> gameCamera =
				GraphicEngine::get()->getSceneManager()->getCameraManager()->getCamera(CameraName::GameScene);
			gameCamera->setFPSLock(false);
		}

	}

}

std::string GameEngine::getGameModeString()
{
	std::string nameStr = "";
	switch (runMode)
	{
	case EngineRunMode::EngineDebugMode:
		nameStr = "Debug Mode";
		break;
	case EngineRunMode::EngineGameMode:
		nameStr = "Game Mode";
		break;
	}
	return nameStr;
}

void GameEngine::OutConsole(std::string s)
{
#ifdef USE_IMGUI
	console.AddLog(s.c_str());
#endif // USE_IMGUI
}

void GameEngine::OutConsole(int value)
{
	std::string s = std::to_string(value);
#ifdef USE_IMGUI
	console.AddLog(s.c_str());
#endif // USE_IMGUI
}

void GameEngine::OutConsole(float value)
{
	std::string s = std::to_string(value);
#ifdef USE_IMGUI
	console.AddLog(s.c_str());
#endif // USE_IMGUI
}
