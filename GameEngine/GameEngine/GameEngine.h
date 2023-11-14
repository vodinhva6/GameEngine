#pragma once
#include <dxgi1_2.h>
#include <d3d11.h>
#include <filesystem>
#include <wrl.h>

#include "DataFormat.h"
#include "SingletonPatternTemplate.h"
#include "FramebufferNumName.h"


enum class EngineRunMode
{
    EngineDebugMode,
    EngineGameMode
};

class GameEngine : public SingletonPatternTemplate <GameEngine>
{
    friend class SingletonPatternTemplate <GameEngine>;
public:
    GameEngine();
    void Init(const HWND* hwnd);
    void Update(float elapsedTime);
    void Render();
    void CreateCOM(ID3D11Device* device, IDXGISwapChain1* swapChain, const SIZE& frameSize);
    void ResizeRenderBegin();
    void ResizeRenderEnd(ID3D11Device* device, IDXGISwapChain1* swapChain, const SIZE& frameSize);
    void DrawAllUninvisibleObject(FrameBufferName name, DrawType type);
    void CastShadow(FrameBufferName name, DrawType type, UINT numIndex);
    void ResetRenderTagertAndDepthStencil();
    void NewScene();
    void SaveScene(std::filesystem::path local = "", bool saveAs = false);
    void OpenScene(std::filesystem::path local, bool* result);
    bool SceneHadSaveLocal();
    void ClearRenderTagertAndDepthStencil();
    void SetContentDirectory(std::filesystem::path directory);
    void DrawSkymap(bool debug = false);
    void SetIBLSkyMap();
    ~GameEngine();
public:
    void CreateVS(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
        ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);
    void CreatePS(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);

public:
    bool CreateRay(pSmartVoid& ray, const VECTOR3& begin, const VECTOR3& end, const VECTOR4& color, int rayType = 0);
    bool LoadSkinnedMesh(std::filesystem::path fileLocal, pSmartVoid& mesh);
    bool CreateSkinnedMeshThumbnail(std::filesystem::path fileName, pSmartVoid* mesh, bool* result);
    bool LoadMaterial(std::filesystem::path fileName, pSmartVoid& material);
    bool LoadAnimator(std::filesystem::path fileName, pSmartVoid& animator);
    bool LoadAnimation(std::filesystem::path fileName, pSmartVoid& animation);

    bool LoadMaterialThumbnail(std::filesystem::path fileName, pSmartVoid* material, bool* result);
    void LoadAnimatorThumbnail(std::filesystem::path fileName, pSmartVoid* Animator, bool* result);


    void LoadAnimationThumbnail(std::filesystem::path fileName, pSmartVoid* Animation, bool* result);
    bool CreateCapsuleDebug(pSmartVoid& cap, const float& height, const float& weight, const VECTOR3& offset);
    bool createSphereDebug(pSmartVoid& sphere, const float& radius, const VECTOR3& offset);
    bool CreateSprite3D(std::filesystem::path local, pSmartVoid& sprite3D);


    bool ReleaseTexture(std::filesystem::path);
    bool ReleaseMesh(std::filesystem::path local);
    bool ReleaseMaterial(std::filesystem::path local);
    bool ReleaseAnimator(std::filesystem::path local);
    bool ReleaseAnimation(std::filesystem::path local);

    bool RebuildMaterial(pSmartVoid material);
    bool RenameMaterial(std::filesystem::path newName, std::filesystem::path oldName);
    bool RenameAnimator(std::filesystem::path newName, std::filesystem::path oldName);
    bool RenameAnimation(std::filesystem::path newName, std::filesystem::path oldName);

    bool DeleteMaterial(std::filesystem::path local);
    bool DeleteAnimator(std::filesystem::path local);

    bool SetMeshDefaultTransform(std::filesystem::path fileLocal, const VECTOR3& posOff, const VECTOR3& rotOff, const VECTOR3& scaOff);
    std::filesystem::path CreateNewDummyMaterial(std::filesystem::path path);
    std::filesystem::path CreateNewAnimator(std::filesystem::path path);


    void SpriteTextOutScreen(int textNo, const std::string contents, const VECTOR2& position, const float& size, const VECTOR4& color, const int& drawTurn, bool late = true);
    void DrawSkinnedMesh(pSmartVoid mesh, const DirectX::XMFLOAT4X4 world, pVoid materialList,
        const VECTOR4& color, const int& drawTurn, DrawStates& drawStates, pVoid keyFrame, FrameBufferName name);
    void DrawSpriteLate(void* sprite,const VECTOR2& position, const VECTOR2& scale,const VECTOR4& color,const VECTOR2& texture_position, const VECTOR2& texture_size,const float& angle, const int& drawTurn);
    void DrawSprite3D(pSmartVoid sprite3D, const DirectX::XMFLOAT4X4 world, const VECTOR4& color, int drawTurn, FrameBufferName name);
    void DrawDebugBounding(pSmartVoid geo, const DirectX::XMFLOAT4X4 world, const VECTOR4& color, const int& type);


    void LoadTextureFromFileMultiThread(std::wstring fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* shader_resource_view, bool* result = nullptr);
    bool LoadTextureFromFile(std::wstring fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, bool* result = nullptr);
    bool LoadTextureFromFile(std::filesystem::path local, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, bool* result = nullptr);
    bool LoadTextureFromFile(std::filesystem::path local, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc);
    bool LoadTextureFromMemory(const void* data, size_t size, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResourceView);

    float getAspectRatio(FrameBufferName name);


    std::string DropData(std::string dataType, int flag = 0);

public:
    pVoid getMousePoint();
    pVoid getControlPad();

public:
    ID3D11Device* getDevice();
    IDXGISwapChain* getSwapChain();
    ID3D11DeviceContext* getDeviceContext();
    ID3D11DeviceContext** getAddrDeviceContext();
    D3D11_VIEWPORT& getViewPort(FrameBufferName name);
    float getElapsedTime();
public:

    pVoid getActorManager();
    pVoid getFrameBufferManager();
    pVoid getCameraManager();
    pVoid getShaderManager();
    pVoid getLightManager();
    pVoid getDepthStencilStateManager();
    pVoid getSamplerStateManager();
    pVoid getBlendStateManager();
    pVoid getRasterizerStateManager();
    pVoid getTextureManager();
    pVoid getMaterialManager();
    pVoid getMeshLoaderManager();
    pVoid getContentBrowser();
    pVoid getAnimatorManager();
private:
    void CheckChangeRunGameMode();

public:
    EngineRunMode getGameMode() { return runMode; }
   
    std::string getGameModeString();
   
    bool getExitGame() { return exitGame; }

    void setExitGame(bool exit) { exitGame = exit; }
    void setGameRunningMode(EngineRunMode mode) { runMode = mode; }


public:
    void OutConsole(std::string s);
    void OutConsole(int value);
    void OutConsole(float value);
private:
    bool exitGame;
    EngineRunMode runMode;

private:
    //std::unique_ptr<ActorManager> actorManager;
    
};

  
