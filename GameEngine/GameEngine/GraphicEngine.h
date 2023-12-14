#pragma once
#include <dxgi1_2.h>
#include "SingletonPatternTemplate.h"
#include "SamplerStateManager.h"
#include "DepthStencilStateManager.h"
#include "BlendStateManager.h"
#include "RasterizerStateManager.h"
#include "ConstantBufferManager.h"
#include "FrameBufferManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "MeshLoaderManager.h"
#include "Sprite3DLoadManager.h"
#include "MaterialManager.h"
#include "SceneManager.h"
#include "AnimatorManager.h"

#include "Sprite.h"
#include "GeometricPrimitive.h"
#include "EffectSeerManager.h"
#include "StaticMesh.h"
#include "BoundingCapsule.h"
#include "BoundingSphere.h"
#include "Ray.h"

class GraphicEngine : public SingletonPatternTemplate< GraphicEngine>
{
    friend class SingletonPatternTemplate< GraphicEngine>;
public:
    GraphicEngine() {}
    void Init();
    void CreateCOM(ID3D11Device* device, IDXGISwapChain1* swapChain,const SIZE& frameSize);
    void Update(float elapsedTime);
    void Render();
    void DrawSkymap(bool debug = false);
    void SetIBLSkyMap();
    void EditSkymap();
    ~GraphicEngine();
public:
    void DrawSkinnedMesh(SkinnedMesh* obj, const DirectX::XMFLOAT4X4 world, 
        std::unordered_map<int64_t, std::shared_ptr<Material>>* materialList, 
        const VECTOR4& color, const int& drawTurn, DrawStates& drawStates, Animation::Keyframe* keyFrame, FrameBufferName slotFrameBuffer);
    void DrawEffect(std::string effName, const VECTOR3& position, const int& drawTurn, int slotFrameBuffer, float size = 1, VECTOR3 rotation = { 0,0,0 }, float angle = 0, VECTOR4 color = { 1,1,1,1 }
    );
    void DrawSprite3D(Sprite3D* sprite3D, const DirectX::XMFLOAT4X4 world, const VECTOR4& color, int drawTurn, FrameBufferName slotFrameBuffer);
    void DrawSpriteLate(Sprite* sprite,
        const VECTOR2& position, const VECTOR2& scale,
        const VECTOR4& color,
        const VECTOR2& texture_position, const VECTOR2& texture_size,
        const float& angle, const int& drawTurn,
        DrawStates& drawStates
    );
    void DrawSpriteEarly(Sprite* sprite,
        const VECTOR2& position, const VECTOR2& scale,
        const VECTOR4& color,
        const VECTOR2& texture_position, const VECTOR2& texture_size,
        const float& angle, const int& drawTurn,
        DrawStates& drawStates
    );
    void DrawSpriteLate(Sprite* sprite,
        const VECTOR2& position, const VECTOR2& scale,
        const VECTOR4& color,
        const VECTOR2& texture_position, const VECTOR2& texture_size,
        const float& angle, const int& drawTurn
    );
    void DrawSpriteEarly(Sprite* sprite,
        const VECTOR2& position, const VECTOR2& scale,
        const VECTOR4& color,
        const VECTOR2& texture_position, const VECTOR2& texture_size,
        const float& angle, const int& drawTurn
    );
    void DrawBackgroudSpriteLate(Sprite* sprite, const VECTOR2& position, const VECTOR2& size, const int& drawTurn);
    void DrawBackgroudSpriteEarly(Sprite* sprite, const VECTOR2& position, const VECTOR2& size, const int& drawTurn);
    void TextOut(int textNo, const std::string contents, const VECTOR2& position, const float& size, const VECTOR4& color, const int& drawTurn, bool late = true);

   
    //void drawDebugBounding(GeometricPrimitive* geo, const DirectX::XMFLOAT4X4 world, const VECTOR4& color, const int& type);
public:
    Sprite* CreateSprite(const wchar_t* filename, ShaderData* shaderData = nullptr);
    bool CreateSprite3D(std::filesystem::path local, pSmartVoid& sprite3D);
    bool CreateSkinnedMesh(std::filesystem::path fileLocal, pSmartVoid& mesh);
    bool LoadMaterial(std::filesystem::path fileName, pSmartVoid& material);
    bool LoadAnimator(std::filesystem::path fileName, pSmartVoid& animator);
    bool LoadAnimation(std::filesystem::path fileName, pSmartVoid& animation);

    void createEffect(const char* filename, std::string effName);
    //BoundingCapsule* createCapsuleDebug(const float& height, const float& weight, const VECTOR3& offset);
    //BoundingSphere* createSphereDebug(const float& radius, const VECTOR3& offset);
    //Ray* createRay(const VECTOR3& begin, const VECTOR3& end, const VECTOR4& color, RayType Type = NON);

public:


    float getAspectRatio(FrameBufferName name);
    D3D11_VIEWPORT& getViewPort(FrameBufferName name);
    
    float getElapsedTime(); 

    void ResetFrameBufferManager() { framebufferManager.reset(); }

    ShaderManager* getShaderManager() { return shaderManager.get(); }
    TextureManager* getTextureManager() { return textureManager.get(); }
    MaterialManager* getMaterialManager() { return materialManager.get(); }
    MeshLoaderManager* getMeshLoaderManager() { return meshLoaderManager.get(); }
    AnimatorManager* getAnimatorManager() { return animatorManager.get(); }

    FrameBufferManager* getFrameBufferManager() { return framebufferManager.get(); }
    SceneManager* getSceneManager() { return sceneManager.get(); }

    SamplerStateManager* getSamplerStateManager() { return samplerStateManager.get(); }
    DepthStencilStateManager* getDepthStencilStateManager() { return depthStencilStateManager.get(); }
    BlendStateManager* getBlendStateManager() { return blendStateManager.get(); }
    RasterizerStateManager* getRasterizerStateManager() { return rasterizerStateManager.get(); }

public:
    bool LoadTextureFromFile(std::wstring fileName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shader_resource_view, bool* result = nullptr);


   
public:
    ID3D11DeviceContext* getDeviceContext() { return immediateContext.Get(); }
    ID3D11DeviceContext** getAddrDeviceContext() { return immediateContext.GetAddressOf(); }
public:
    void ResetRenderTagertAndDepthStencil();
    void RemoveQueneObject();

public:
    void Draw2D(bool late);
    void DrawAllUninvisibleObject(int frameBufferSlot, DrawType type);
    void CastShadow(int frameBufferSlot, DrawType type, UINT numIndex);

private:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;


public:
    void CreateRenderTagert(ID3D11Device* device, IDXGISwapChain1* swapChain);
    void CreateDepthStencilView(ID3D11Device* device, IDXGISwapChain1* swapChain, const SIZE& frameSize);
    void CreateSamplerState(ID3D11Device* device);
    void CreateDepthStencilState(ID3D11Device* device);
    void CreateBlendState(ID3D11Device* device);
    void CreateRasterizerState(ID3D11Device* device);
    void CreateConstantBuffer(ID3D11Device* device);
    void CreateFramebuffer(ID3D11Device* device, IDXGISwapChain1* swapChain);
    void CreateShaderManager(ID3D11Device* device);
    void CreateTextureManager();
    void CreateMaterialManager();
    void CreateSceneManager();
    void CreateMeshLoaderManager();
    void CreateSprite3DLoaderManager();
    void CreateTextFont();
    void CreateAnimatorManager();

    void NewScene();
    void SaveScene(std::filesystem::path local, bool saveAs);
    void OpenScene(std::filesystem::path local);
    bool SceneHadSaveLocal();
    void ClearRenderTagertAndDepthStencil(const VECTOR4& color = { 0,0,0,1 });
private:
    void SetSlotPSConstantBuffer();
    void SetSlotVSConstantBuffer();
    void UpdateResources();
    void SetSlotSampler();
   

   

private:
    std::unique_ptr<SamplerStateManager>      samplerStateManager;
    std::unique_ptr<DepthStencilStateManager> depthStencilStateManager;
    std::unique_ptr<BlendStateManager       > blendStateManager;
    std::unique_ptr<RasterizerStateManager  > rasterizerStateManager;
    std::unique_ptr<ConstantBufferManager   > constantBufferManager;
    std::unique_ptr<FrameBufferManager      > framebufferManager;
    std::unique_ptr<ShaderManager           > shaderManager;
    std::unique_ptr<TextureManager          > textureManager;
    std::unique_ptr<MeshLoaderManager       > meshLoaderManager;
    std::unique_ptr<SceneManager            > sceneManager;
    std::unique_ptr<Sprite3DLoadManager     > sprite3DLoadManager;
    std::unique_ptr<MaterialManager         > materialManager;
    std::unique_ptr<AnimatorManager         > animatorManager;


private:
    const wchar_t* textFonts[8] =
    {
        L"./Data/Asset/Fonts/font0.png",
        L"./Data/Asset/Fonts/font1.png",
        L"./Data/Asset/Fonts/font2.png",
        L"./Data/Asset/Fonts/font3.png",
        L"./Data/Asset/Fonts/font4.png",
        L"./Data/Asset/Fonts/font5.png",
        L"./Data/Asset/Fonts/font6.png",
        L"./Data/Asset/Fonts/font7.png",
    };
    VECTOR2 text_fonts_size[8] =
    {
        {36,36},
        {36,36},
        {36,36},
        {36,36},
        {36,36},
        {36,36},
        {36,36},
        {100,100},
    };

    class SpritesData;
    class GeometricPrimitiveData;
    class StaticMeshData;
    class SkinnedMeshData;
    class EffectData;
    class Sprite3DData;
    class ObjectDrawData
    {
    public:
        virtual SpritesData* getSpritesData() { return nullptr; }
        virtual GeometricPrimitiveData* getGeometricPrimitiveData() { return nullptr; }
        virtual StaticMeshData* getStaticMeshData() { return nullptr; }
        virtual SkinnedMeshData* getSkinnedMeshData() { return nullptr; }
        virtual EffectData* getEffectData() { return nullptr; }
        virtual Sprite3DData* getSprite3D() { return nullptr; }
        virtual ~ObjectDrawData() {}
        DrawStates drawState_;
        int drawTurn_;
    };
    class SpritesData :public ObjectDrawData
    {
    public:
        SpritesData* getSpritesData() override { return this; }
        SpritesData(Sprite* sprite, const VECTOR2& position,
            const VECTOR2& size, const VECTOR4& color,
            const VECTOR2& texture_position, const VECTOR2& texture_size,
            const float& angle, int drawTurn,
            DrawStates drawState, std::string contents
        ) :
            sprite_(sprite), position_(position), size_(size), color_(color), texture_position_(texture_position),
            texture_size_(texture_size), angle_(angle), contents_(contents)
        {
            ObjectDrawData::drawTurn_ = drawTurn;
            ObjectDrawData::drawState_ = drawState;
        }

        SpritesData& operator=(const SpritesData& data)
        {
            this->sprite_ = data.sprite_;
            this->position_ = data.position_;
            this->size_ = data.size_;
            this->color_ = data.color_;
            this->texture_position_ = data.texture_position_;
            this->texture_size_ = data.texture_size_;
            this->angle_ = data.angle_;
            this->drawTurn_ = data.drawTurn_;
            this->drawState_ = data.drawState_;
            this->contents_ = data.contents_;
            return *this;
        }

        Sprite* sprite_;
        VECTOR2 position_;
        VECTOR2 size_;
        VECTOR4 color_;
        VECTOR2 texture_position_;
        VECTOR2 texture_size_;
        float angle_;


        std::string contents_;



        ~SpritesData() override {}
    };
    /*class GeometricPrimitiveData : public ObjectDrawData
    {
    public:
        GeometricPrimitiveData* getGeometricPrimitiveData() override { return this; }
        GeometricPrimitiveData() {}
        GeometricPrimitiveData(GeometricPrimitive* geometric, const DirectX::XMFLOAT4X4& world, const VECTOR4& color, DrawStates drawState, const int& type)
            :geometric_(geometric), world_(world), color_(color), type_(type)
        {
            drawState_ = (drawState);
        }

        GeometricPrimitiveData& operator=(const GeometricPrimitiveData& data)
        {
            this->geometric_ = data.geometric_;
            this->world_ = data.world_;
            this->color_ = data.color_;
            this->drawTurn_ = data.drawTurn_;
            this->drawState_ = data.drawState_;
            this->type_ = data.type_;
            return *this;
        }

        GeometricPrimitive* geometric_;
        DirectX::XMFLOAT4X4 world_;
        VECTOR4 color_;
        int type_;

        ~GeometricPrimitiveData() {}
    };*/
    class StaticMeshData :public ObjectDrawData
    {
    public:
        StaticMeshData* getStaticMeshData() override { return this; }
        StaticMeshData(StaticMesh* staticMesh, const VECTOR3& position,
            const VECTOR3& rotation,
            const VECTOR3& size, const VECTOR4& color, int drawTurn, DrawStates drawState,
            const bool& drawBoudingBox
        ) :staticMesh_(staticMesh), rotation_(rotation),
            position_(position), size_(size), color_(color),
            drawBoudingBox_(drawBoudingBox)
        {
            ObjectDrawData::drawTurn_ = drawTurn;
            ObjectDrawData::drawState_ = drawState;
        }

        StaticMeshData& operator=(const StaticMeshData& data)
        {
            this->staticMesh_ = data.staticMesh_;
            this->position_ = data.position_;
            this->size_ = data.size_;
            this->color_ = data.color_;
            this->drawTurn_ = data.drawTurn_;
            this->drawState_ = data.drawState_;
            this->drawBoudingBox_ = data.drawBoudingBox_;
            return *this;
        }

        StaticMesh* staticMesh_;
        VECTOR3 position_;
        VECTOR3 rotation_;
        VECTOR3 size_;
        VECTOR4 color_;
        bool drawBoudingBox_;


        ~StaticMeshData() override {}
    };
    class SkinnedMeshData :public ObjectDrawData
    {
    public:
        SkinnedMeshData* getSkinnedMeshData() override { return this; }
    
        SkinnedMeshData(SkinnedMesh* skinnedMesh, DirectX::XMFLOAT4X4 world,
            std::unordered_map<int64_t, std::shared_ptr<Material>>* materialList,
            const VECTOR4& color, int drawTurn,
            DrawStates drawState, Animation::Keyframe* keyFrame = nullptr) :
            skinnedMesh_(skinnedMesh), world_(world), color_(color),
            materialList(materialList), keyFrame(keyFrame)
        {
            ObjectDrawData::drawTurn_ = drawTurn;
            ObjectDrawData::drawState_ = drawState;
        }
        SkinnedMeshData& operator=(const SkinnedMeshData& data)
        {
            this->skinnedMesh_ = data.skinnedMesh_;
            this->world_ = data.world_;
            this->color_ = data.color_;
            this->drawTurn_ = data.drawTurn_;
            this->drawState_ = data.drawState_;
            this->materialList = data.materialList;
            this->keyFrame = data.keyFrame;
            return *this;
        }
    
        SkinnedMesh* skinnedMesh_;
        std::unordered_map<int64_t, std::shared_ptr<Material>>* materialList;
        Animation::Keyframe* keyFrame;
        DirectX::XMFLOAT4X4 world_;
        VECTOR4 color_;
    };
    class EffectData : public ObjectDrawData
    {
    public:

        EffectData* getEffectData() { return this; }

        EffectData(Effect* eff, const VECTOR3& position, const VECTOR3& rotation, const VECTOR4& color,
            const float& scale, const int& drawTurn, const float& angle, DrawStates drawState)
        {
            this->eff_ = eff;
            this->position_ = position;
            this->rotation_ = rotation;
            this->color_ = color;
            this->scale_ = scale;
            this->angle_ = angle;
            this->drawState_ = drawState;
            drawTurn_ = drawTurn;
        }
        EffectData& operator=(const EffectData& data)
        {
            this->eff_ = data.eff_;
            this->position_ = data.position_;
            this->rotation_ = data.rotation_;
            this->color_ = data.color_;
            this->scale_ = data.scale_;
            this->drawTurn_ = data.drawTurn_;
            this->angle_ = data.angle_;
            this->drawState_ = data.drawState_;
        }
        Effect* eff_;
        VECTOR3 position_;
        VECTOR3 rotation_;
        VECTOR4 color_;
        float angle_;
        float scale_;


    };
    class Sprite3DData : public ObjectDrawData
    {
    public:

        Sprite3DData(const Sprite3DData& data)
        {
            this->sprite3D = data.sprite3D;
            this->world = data.world;
            this->drawTurn_ = data.drawTurn_;
            this->color = data.color;
            this->drawState_ = data.drawState_;
        }
        Sprite3DData(Sprite3D* sprite3D, DirectX::XMFLOAT4X4 world, const VECTOR4& color, int drawTurn, DrawStates drawStates) :
            sprite3D(sprite3D), world(world), color(color)
        {
            drawState_ = drawStates;
            drawTurn_ = drawTurn;
        }
        Sprite3DData& operator=(const Sprite3DData& data)
        {
            this->sprite3D = data.sprite3D;
            this->world = data.world;
            this->drawTurn_ = data.drawTurn_;
            this->color = data.color;
            this->drawState_ = data.drawState_;
            return *this;
        }
        Sprite3DData* getSprite3D() override { return this; }
        Sprite3D* sprite3D;
        DirectX::XMFLOAT4X4 world;
        VECTOR4 color;
    };


    std::map<std::string, Effect*> effects;
   
    std::vector<std::shared_ptr<Sprite>> sprites;
    //std::vector<std::shared_ptr<GeometricPrimitive>> geometricPrimitive;


    std::vector<std::vector<ObjectDrawData*>> uninvisibleDataObjectsFrameBuffer;
    std::vector<ObjectDrawData*> invisibleDataObjects;
    std::vector<ObjectDrawData*> Object2DListEarly;
    std::vector<ObjectDrawData*> Object2DListLate;

private:
    void DeleteAllEffect();
};


