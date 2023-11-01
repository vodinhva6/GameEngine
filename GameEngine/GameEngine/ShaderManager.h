#pragma once
#include <d3d11.h>

enum GEOMETRYSHADER
{
    CSM_SKINNEDMESH_GEOMETRYSHADER,
};

enum VERTEXSHADER
{
    SPRITE_VEXTEXSHADER,
    SPRITE3D_VEXTEXSHADER,
    SKINNEDMESH_LARMBERT_VEXTEXSHADER,
    SKINNEDMESH_LARMBERT_VEXTEXSHADER_DEBUG,
    SKINNEDMESH_LARMBERTWITHSHADOW_VEXTEXSHADER,
    SKINNEDMESH_CREATESHADOW_VEXTEXSHADER,
    FULLSCREENQUAD_VERTEXSHADER,
    GEOMETRIC_DEBUG_VEXTEXSHADER,
    GEOMETRIC_VEXTEXSHADER,
    SKINNEDMESH_DEBUG_VEXTEXSHADER,
    SKINNEDMESH_DEBUG_THUMBNAIL_VEXTEXSHADER,
    CSM_SKINNEDMESH_VERTEXSHADER,
    SKINNEDMESH_POINTLIGHT_SHADOWMAP_VERTEXSHADER,
};

enum PIXELSHADER
{
    SPRITE_PIXELSHADER,
    SPRITE3D_PIXELSHADER,
    SKINNEDMESH_OFSOFTSHADOW_PIXELSHADER,
    SKINNEDMESH_LARMBERT_PIXELSHADER,
    MESH_BLACK_NON_TEXTURE_PIXELSHADER,
    SKINNEDMESH_LARMBERT_PIXELSHADER_DEBUG,
    SKINNEDMESH_LARMBERTWITHSHADOW_PIXELSHADER,
    FULLSCREENQUAD_WITHMOTIONBLUR_PIXELSHADER,
    FULLSCREENQUAD_WITHVOLUMETRICLIGHTSCATTERING_PIXELSHADER,
    FULLSCREENQUAD_PIXELSHADER,
    FULLSCREENQUAD_FROMTWOFRAMEBUFFER_PIXELSHADER,
    FULLSCREENQUAD_ADDCOLORVOLUMETRICLIGHTSCATTERING_PIXELSHADER,
    FULLSCREENQUAD_FROMTWOFRAMEBUFFER_DEPTHSTENCIL_PIXELSHADER,
    GEOMETRIC_DEBUG_PIXELSHADER,
    GEOMETRIC_PIXELSHADER,
    FULLSCREENQUAD_SKYMAP_PIXELSHADER,
    FULLSCREENQUAD_SKYMAP_PIXELSHADER_DEBUG,
    SKINNEDMESH_DEBUG_PIXELSHADER,
    SKINNEDMESH_DEBUG_THUMBNAIL_PIXELSHADER,
    MESHLIGHTWITHEMISSION_PIXELSHADER,
    CSM_SKINNEDMESH_PIXELSHADER,


    GLOW_EXTRACTION_PIXELSHADER,
    GAUSSIAN_BLUR_DOWNSAMPLING_PIXELSHADER, 
    GAUSSIAN_BLUR_HORIZONTAL_PIXELSHADER,
    GAUSSIAN_BLUR_VERTICAL_PIXELSHADER,
    GAUSSIAN_BLUR_UPSAMPLING_PIXELSHADER,


    SHADOW_APPLY_PIXELSHADER
};
enum INPUTLAYOUT
{
    SPRITE,
    SPRITE3D,
    SKINNEDMESH,
    GEOMETRIC_INPUT
};



class ShaderManager
{
public:
    ShaderManager() {}
    HRESULT createVSFromFile_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
         ID3D11InputLayout * *input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);
    HRESULT createPSFromFile_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);
    HRESULT createGsFrom_cso(ID3D11Device* device, const char* cso_name, ID3D11GeometryShader** geometry_shader);
    void SetDrawStateDraw2D(ID3D11DeviceContext* dcContext);

    void BeginDraw2D(ID3D11DeviceContext* dcContext);
    void BeginDraw3DSprite(ID3D11DeviceContext* dcContext);
    void BeginDraw3D(ID3D11DeviceContext* dcContext);
    void BeginDraw3DWithShadow(ID3D11DeviceContext* dcContext);
    void BeginDraw3DCreateDepthBufferOfShadow(ID3D11DeviceContext* dcContext);
    void BeginBlitFullScreenQuadWithVolumetricLightScattering(ID3D11DeviceContext* dcContext);
    void BeginMeshWithBlackNONTexture(ID3D11DeviceContext* dcContext);
    void BeginBlitFullScreenQuadWithMotionBlur(ID3D11DeviceContext* dcContext);
    void BeginBlitFullScreenQuad(ID3D11DeviceContext* dcContext);
    void BeginBlitAddColorFromTwoFramebufferFullScreenQuad(ID3D11DeviceContext* dcContext, bool depthStencil = false);
    void BeginBlitAddColorVolumetricLightScatteringFullScreenQuad(ID3D11DeviceContext* dcContext, bool depthStencil = false);
    void BeginDraw3DCreateSoftShadow(ID3D11DeviceContext* dcContext, ID3D11DepthStencilState* depth,
        ID3D11BlendState* blend, ID3D11RasterizerState* rasze);
    void BeginGeometricDrawDebug(ID3D11DeviceContext* dcContext);
    void BeginSkinnedMeshDrawDebug(ID3D11DeviceContext* dcContext);
    void BeginMeshLightWithEmissionDebug(ID3D11DeviceContext* dcContext);
    void BeginMeshLightWithEmission(ID3D11DeviceContext* dcContext);
    void BeginSkinnedMeshDrawDebugThumbnail(ID3D11DeviceContext* dcContext);
    void BeginBlitFullScreenQuadSkymap(ID3D11DeviceContext* dcContext, bool debug = false);
    void End(ID3D11DeviceContext* dcContext);
    void CreateShaderAllLayout(ID3D11Device* dcContext);

    void BeginGlowExtraction(ID3D11DeviceContext* dcContext);
    void BeginDownSampling(ID3D11DeviceContext* dcContext);
    void BeginGaussianBlurVertical(ID3D11DeviceContext* dcContext);
    void BeginGaussianBlurHorizontal(ID3D11DeviceContext* dcContext);
    void BeginUpSampling(ID3D11DeviceContext* dcContext);


    void BeginCastShadow(ID3D11DeviceContext* dcContext);
    void BeginPointLightCastShadow(ID3D11DeviceContext* dcContext);
    void BeginShadowApplyPostEffect(ID3D11DeviceContext* dcContext);
    ~ShaderManager() {}
private:
    

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader[20];
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader[40];
    Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader[10];
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout[20];
};

