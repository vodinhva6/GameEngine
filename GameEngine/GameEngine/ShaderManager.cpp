#include <wrl.h>
#include <string>
#include <map>
#include <memory>
#include "GameEngine.h"
#include "Misc.h"
#include "ShaderManager.h"
#include "DepthStencilStateManager.h"
#include "BlendStateManager.h"
#include "RasterizerStateManager.h"
HRESULT ShaderManager::createVSFromFile_cso(ID3D11Device* device , const char* cso_name, ID3D11VertexShader** vertex_shader, ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);
    
    std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    if (input_layout)
    {
        hr = device->CreateInputLayout(input_element_desc, num_elements,
            cso_data.get(), cso_sz, input_layout);
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    } 

    return hr;
}
HRESULT ShaderManager::createGsFrom_cso(ID3D11Device* device, const char* cso_name, ID3D11GeometryShader** geometry_shader)
{
    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);

    HRESULT hr{ S_OK };
    hr = device->CreateGeometryShader(cso_data.get(), cso_sz, nullptr, geometry_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    return hr;
}

HRESULT ShaderManager::createPSFromFile_cso(ID3D11Device* device , const char* cso_name, ID3D11PixelShader** pixel_shader)
{
    FILE * fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");
    
     fseek(fp, 0, SEEK_END);
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);
    
    std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);
    
     HRESULT hr{ S_OK };
    hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    
     return hr;
}



void ShaderManager::CreateShaderAllLayout(ID3D11Device* device)
{
    HRESULT hr;

    {
        D3D11_INPUT_ELEMENT_DESC input_element_desc[]
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
           D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
           D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };


        {

            hr = createVSFromFile_cso(device, "./Data/Shaders/GeometricPrimitiveDebugShader_vs.cso",
                vertexShader[VERTEXSHADER::GEOMETRIC_DEBUG_VEXTEXSHADER].ReleaseAndGetAddressOf(),
                inputLayout[INPUTLAYOUT::GEOMETRIC_INPUT].ReleaseAndGetAddressOf(), input_element_desc, _countof(input_element_desc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            hr = createPSFromFile_cso(device, "./Data/Shaders/GeometricPrimitiveDebugShader_ps.cso",
                pixelShader[PIXELSHADER::GEOMETRIC_DEBUG_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }


        {

            hr = createVSFromFile_cso(device, "./Data/Shaders/geometric_primitive_vs.cso",
                vertexShader[VERTEXSHADER::GEOMETRIC_VEXTEXSHADER].ReleaseAndGetAddressOf(),
                inputLayout[INPUTLAYOUT::GEOMETRIC_INPUT].ReleaseAndGetAddressOf(), input_element_desc, _countof(input_element_desc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            hr = createPSFromFile_cso(device, "./Data/Shaders/geometric_primitive_ps.cso",
                pixelShader[PIXELSHADER::GEOMETRIC_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }

        


    }
    //Sprite
    {
        D3D11_INPUT_ELEMENT_DESC input_element_desc[]
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        hr = createVSFromFile_cso(device, "./Data/Shaders/sprite_vs.cso",
            vertexShader[VERTEXSHADER::SPRITE_VEXTEXSHADER].ReleaseAndGetAddressOf(), inputLayout[INPUTLAYOUT::SPRITE].ReleaseAndGetAddressOf(), input_element_desc, _countof(input_element_desc));
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        hr = createPSFromFile_cso(device, "./Data/Shaders/sprite_ps.cso",
            pixelShader[PIXELSHADER::SPRITE_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    //3DSprite
    {
        D3D11_INPUT_ELEMENT_DESC input_element_desc[]
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        hr = createVSFromFile_cso(device, "./Data/Shaders/Sprite3D_vs.cso",
            vertexShader[VERTEXSHADER::SPRITE3D_VEXTEXSHADER].ReleaseAndGetAddressOf(), inputLayout[INPUTLAYOUT::SPRITE3D].ReleaseAndGetAddressOf(), input_element_desc, _countof(input_element_desc));
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        hr = createPSFromFile_cso(device, "./Data/Shaders/Sprite3D_ps.cso",
            pixelShader[PIXELSHADER::SPRITE3D_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    //SkinnedMesh
    {

        D3D11_INPUT_ELEMENT_DESC input_element_desc[]
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT },
            { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT },
            { "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT },
            { "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT },

        };

        //
        {
            hr = createVSFromFile_cso(device, "./Data/Shaders/skinned_mesh_vs.cso",
                vertexShader[VERTEXSHADER::SKINNEDMESH_LARMBERT_VEXTEXSHADER].ReleaseAndGetAddressOf(),
                inputLayout[INPUTLAYOUT::SKINNEDMESH].ReleaseAndGetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

            hr = createPSFromFile_cso(device, "./Data/Shaders/skinned_mesh_ps.cso",
                pixelShader[PIXELSHADER::SKINNEDMESH_LARMBERT_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }

        //
        {
            hr = createVSFromFile_cso(device, "./Data/Shaders/SkinnedMeshWithShadow_VS.cso",
                vertexShader[VERTEXSHADER::SKINNEDMESH_LARMBERTWITHSHADOW_VEXTEXSHADER].ReleaseAndGetAddressOf(),
                inputLayout[INPUTLAYOUT::SKINNEDMESH].ReleaseAndGetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

            hr = createPSFromFile_cso(device, "./Data/Shaders/SkinnedMeshWithShadow_PS.cso",
                pixelShader[PIXELSHADER::SKINNEDMESH_LARMBERTWITHSHADOW_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }


        //
        {
            hr = createVSFromFile_cso(device, "./Data/Shaders/SkinnedMeshOfShadow_VS.cso",
                vertexShader[VERTEXSHADER::SKINNEDMESH_CREATESHADOW_VEXTEXSHADER].ReleaseAndGetAddressOf(),
                inputLayout[INPUTLAYOUT::SKINNEDMESH].ReleaseAndGetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



            hr = createPSFromFile_cso(device, "./Data/Shaders/SkinnedMeshCreateSoftShadow_PS.cso",
                pixelShader[PIXELSHADER::SKINNEDMESH_OFSOFTSHADOW_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        }

        {
            hr = createVSFromFile_cso(device, "./Data/Shaders/SkinnedMeshDebug_vs.cso",
                vertexShader[VERTEXSHADER::SKINNEDMESH_DEBUG_VEXTEXSHADER].ReleaseAndGetAddressOf(),
                inputLayout[INPUTLAYOUT::SKINNEDMESH].ReleaseAndGetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            hr = createPSFromFile_cso(device, "./Data/Shaders/SkinnedMeshDebug_ps.cso",
                pixelShader[PIXELSHADER::SKINNEDMESH_DEBUG_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }
        {
            hr = createVSFromFile_cso(device, "./Data/Shaders/SkinnedMeshDebugThumbnail_vs.cso",
                vertexShader[VERTEXSHADER::SKINNEDMESH_DEBUG_THUMBNAIL_VEXTEXSHADER].ReleaseAndGetAddressOf(),
                inputLayout[INPUTLAYOUT::SKINNEDMESH].ReleaseAndGetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            hr = createPSFromFile_cso(device, "./Data/Shaders/SkinnedMeshDebugThumbnail_ps.cso",
                pixelShader[PIXELSHADER::SKINNEDMESH_DEBUG_THUMBNAIL_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }


        {
            hr = createPSFromFile_cso(device, "./Data/Shaders/MeshLightWithEmission_ps.cso",
                pixelShader[PIXELSHADER::MESHLIGHTWITHEMISSION_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }

        {
            hr = createPSFromFile_cso(device, "./Data/Shaders/MeshWithBlackNONTexture_ps.cso",
                pixelShader[PIXELSHADER::MESH_BLACK_NON_TEXTURE_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            
        }

        {
            hr = createVSFromFile_cso(device, "./Data/Shaders/CSMSkinnedMesh_vs.cso",
                vertexShader[VERTEXSHADER::CSM_SKINNEDMESH_VERTEXSHADER].ReleaseAndGetAddressOf(), 
                inputLayout[INPUTLAYOUT::SKINNEDMESH].ReleaseAndGetAddressOf(),input_element_desc, ARRAYSIZE(input_element_desc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            hr = createVSFromFile_cso(device, "./Data/Shaders/SkinnedMesh_PointLight_vs.cso",
                vertexShader[VERTEXSHADER::SKINNEDMESH_POINTLIGHT_SHADOWMAP_VERTEXSHADER].ReleaseAndGetAddressOf(),
                inputLayout[INPUTLAYOUT::SKINNEDMESH].ReleaseAndGetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            hr = createPSFromFile_cso(device, "./Data/Shaders/CSMSkinnedMesh_ps.cso",
                pixelShader[PIXELSHADER::CSM_SKINNEDMESH_PIXELSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
            hr = createGsFrom_cso(device, "./Data/Shaders/CSMSkinnedMesh_gs.cso",
                geometryShader[GEOMETRYSHADER::CSM_SKINNEDMESH_GEOMETRYSHADER].ReleaseAndGetAddressOf());
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        }

    }
    //FULLSCREENQUAD
    {

        hr = createVSFromFile_cso(device, "./Data/Shaders/FullscreenQuad_vs.cso", 
            vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].ReleaseAndGetAddressOf(), nullptr, nullptr, 0);
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        hr = createPSFromFile_cso(device, "./Data/Shaders/FullscreenQuad_ps.cso", 
            pixelShader[PIXELSHADER::FULLSCREENQUAD_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        hr = createPSFromFile_cso(device, "./Data/Shaders/FullscreenQuadWithMotionBlur_PS.cso",
            pixelShader[PIXELSHADER::FULLSCREENQUAD_WITHMOTIONBLUR_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        hr = createPSFromFile_cso(device, "./Data/Shaders/FullScreenQuadVolumetricLightScattering_ps.cso",
            pixelShader[PIXELSHADER::FULLSCREENQUAD_WITHVOLUMETRICLIGHTSCATTERING_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        hr = createPSFromFile_cso(device, "./Data/Shaders/AddColorTwoFramebuffer_ps.cso",
            pixelShader[PIXELSHADER::FULLSCREENQUAD_FROMTWOFRAMEBUFFER_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        hr = createPSFromFile_cso(device, "./Data/Shaders/AddColorVolumetricLightScatteringFullScreenQuad_ps.cso",
            pixelShader[PIXELSHADER::FULLSCREENQUAD_ADDCOLORVOLUMETRICLIGHTSCATTERING_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        hr = createPSFromFile_cso(device, "./Data/Shaders/AddColorTwoFramebufferDepthStencil_ps.cso",
            pixelShader[PIXELSHADER::FULLSCREENQUAD_FROMTWOFRAMEBUFFER_DEPTHSTENCIL_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        hr = createPSFromFile_cso(device, "./Data/Shaders/Skymap_ps.cso",
            pixelShader[PIXELSHADER::FULLSCREENQUAD_SKYMAP_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        hr = createPSFromFile_cso(device, "./Data/Shaders/SkymapDebug_ps.cso",
            pixelShader[PIXELSHADER::FULLSCREENQUAD_SKYMAP_PIXELSHADER_DEBUG].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        
        hr = createPSFromFile_cso(device, "./Data/Shaders/shadowApply_ps.cso",
            pixelShader[PIXELSHADER::SHADOW_APPLY_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    }
  
    //BLOOM
    {
        hr = createPSFromFile_cso(device, "./Data/Shaders/glowExtraction_ps.cso",
            pixelShader[PIXELSHADER::GLOW_EXTRACTION_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        hr = createPSFromFile_cso(device, "./Data/Shaders/gaussianBlurDownsampling_ps.cso",
            pixelShader[PIXELSHADER::GAUSSIAN_BLUR_DOWNSAMPLING_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        hr = createPSFromFile_cso(device, "./Data/Shaders/gaussianBlurHorizontal_ps.cso",
            pixelShader[PIXELSHADER::GAUSSIAN_BLUR_HORIZONTAL_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        hr = createPSFromFile_cso(device, "./Data/Shaders/gaussianBlurVertical_ps.cso",
            pixelShader[PIXELSHADER::GAUSSIAN_BLUR_VERTICAL_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        hr = createPSFromFile_cso(device, "./Data/Shaders/gaussianBlurUpsampling_ps.cso",
            pixelShader[PIXELSHADER::GAUSSIAN_BLUR_UPSAMPLING_PIXELSHADER].ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    }
}

void ShaderManager::BeginGlowExtraction(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());

    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);

    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::GLOW_EXTRACTION_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);

}

void ShaderManager::BeginDownSampling(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());

    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);

    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::GAUSSIAN_BLUR_DOWNSAMPLING_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginGaussianBlurVertical(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());

    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);

    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::GAUSSIAN_BLUR_VERTICAL_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginGaussianBlurHorizontal(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());

    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);

    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::GAUSSIAN_BLUR_HORIZONTAL_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginUpSampling(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());

    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);

    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::GAUSSIAN_BLUR_UPSAMPLING_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginCastShadow(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());

    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullNON_DrawTriagles);

    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::CSM_SKINNEDMESH_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::CSM_SKINNEDMESH_PIXELSHADER].Get(), nullptr, 0);
    dcContext->GSSetShader(geometryShader[GEOMETRYSHADER::CSM_SKINNEDMESH_GEOMETRYSHADER].Get(), nullptr, 0);

    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginPointLightCastShadow(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());

    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullNON_DrawTriagles);

    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_POINTLIGHT_SHADOWMAP_VERTEXSHADER].Get(), nullptr, 0);
    //dcContext->PSSetShader(pixelShader[PIXELSHADER::CSM_SKINNEDMESH_PIXELSHADER].Get(), nullptr, 0);
    dcContext->GSSetShader(geometryShader[GEOMETRYSHADER::CSM_SKINNEDMESH_GEOMETRYSHADER].Get(), nullptr, 0);

    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginShadowApplyPostEffect(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());

    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullNON_DrawTriagles);

    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::SHADOW_APPLY_PIXELSHADER].Get(), nullptr, 0);

    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::SetDrawStateDraw2D(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());
   
    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);

}


void ShaderManager::BeginDraw2D(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SPRITE].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SPRITE_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::SPRITE_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);


}

void ShaderManager::BeginDraw3DSprite(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());


    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    //ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullNON_DrawTriagles);


    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SPRITE3D].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SPRITE3D_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::SPRITE3D_PIXELSHADER].Get(), nullptr, 0);

    //dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginDraw3D(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullNON_DrawTriagles);

    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_LARMBERT_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::SKINNEDMESH_LARMBERT_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);

}

void ShaderManager::BeginDraw3DWithShadow(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_LARMBERTWITHSHADOW_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::SKINNEDMESH_LARMBERTWITHSHADOW_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginDraw3DCreateDepthBufferOfShadow(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_CREATESHADOW_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->PSSetShader(nullptr, nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginBlitFullScreenQuadWithVolumetricLightScattering(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::FULLSCREENQUAD_WITHVOLUMETRICLIGHTSCATTERING_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);

}

void ShaderManager::BeginMeshWithBlackNONTexture(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullNON_DrawTriagles);

    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_LARMBERT_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::MESH_BLACK_NON_TEXTURE_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginBlitFullScreenQuadWithMotionBlur(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::FULLSCREENQUAD_WITHMOTIONBLUR_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);

}

void ShaderManager::BeginBlitFullScreenQuad(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::FULLSCREENQUAD_PIXELSHADER].Get(), nullptr, 0);


    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);

   
}


void ShaderManager::BeginBlitAddColorFromTwoFramebufferFullScreenQuad(ID3D11DeviceContext* dcContext, bool depthStencil)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);

    ID3D11PixelShader* pixelShader = this->pixelShader[PIXELSHADER::FULLSCREENQUAD_FROMTWOFRAMEBUFFER_PIXELSHADER].Get();
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    if(depthStencil)  
        pixelShader = this->pixelShader[PIXELSHADER::FULLSCREENQUAD_FROMTWOFRAMEBUFFER_DEPTHSTENCIL_PIXELSHADER].Get();
    dcContext->PSSetShader(pixelShader, nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginBlitAddColorVolumetricLightScatteringFullScreenQuad(ID3D11DeviceContext* dcContext, bool depthStencil)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);

    ID3D11PixelShader* pixelShader = this->pixelShader[PIXELSHADER::FULLSCREENQUAD_ADDCOLORVOLUMETRICLIGHTSCATTERING_PIXELSHADER].Get();
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    if (depthStencil)
        pixelShader = this->pixelShader[PIXELSHADER::FULLSCREENQUAD_FROMTWOFRAMEBUFFER_DEPTHSTENCIL_PIXELSHADER].Get();
    dcContext->PSSetShader(pixelShader, nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginDraw3DCreateSoftShadow(ID3D11DeviceContext* dcContext, ID3D11DepthStencilState* depth, ID3D11BlendState* blend, ID3D11RasterizerState* rasze)
{
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_LARMBERTWITHSHADOW_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->PSSetShader(pixelShader[PIXELSHADER::SKINNEDMESH_OFSOFTSHADOW_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginGeometricDrawDebug(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::GEOMETRIC_INPUT].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::GEOMETRIC_DEBUG_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::GEOMETRIC_DEBUG_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginSkinnedMeshDrawDebug(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullNON_DrawTriagles);


    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_DEBUG_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::SKINNEDMESH_DEBUG_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginMeshLightWithEmissionDebug(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    //ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_DEBUG_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::MESHLIGHTWITHEMISSION_PIXELSHADER].Get(), nullptr, 0);

    //dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginMeshLightWithEmission(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_LARMBERT_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::MESHLIGHTWITHEMISSION_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginSkinnedMeshDrawDebugThumbnail(ID3D11DeviceContext* dcContext)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());



    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOn_DepthWriteOn);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::AlphaBlendAlphaCoverage);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->IASetInputLayout(inputLayout[INPUTLAYOUT::SKINNEDMESH].Get());
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::SKINNEDMESH_DEBUG_THUMBNAIL_VEXTEXSHADER].Get(), nullptr, 0);
    dcContext->PSSetShader(pixelShader[PIXELSHADER::SKINNEDMESH_DEBUG_THUMBNAIL_PIXELSHADER].Get(), nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}

void ShaderManager::BeginBlitFullScreenQuadSkymap(ID3D11DeviceContext* dcContext, bool debug)
{
    DepthStencilStateManager* depManager = GetFrom<DepthStencilStateManager>(GameEngine::get()->getDepthStencilStateManager());
    BlendStateManager* blendManager = GetFrom<BlendStateManager>(GameEngine::get()->getBlendStateManager());
    RasterizerStateManager* rasManager = GetFrom<RasterizerStateManager>(GameEngine::get()->getRasterizerStateManager());

    ID3D11DepthStencilState* depth = depManager->getDepthStencill(DepthStencilStates::DepthTestOff_DepthWriteOff);
    ID3D11BlendState* blend = blendManager->getBlendState(BlendStates::Alpha);
    ID3D11RasterizerState* rasze = rasManager->getRasterizerState(RasterizerStates::CullBack_DrawTriagles);


    dcContext->IASetInputLayout(nullptr);
    dcContext->VSSetShader(vertexShader[VERTEXSHADER::FULLSCREENQUAD_VERTEXSHADER].Get(), nullptr, 0);
    ID3D11PixelShader* pxShader = pixelShader[PIXELSHADER::FULLSCREENQUAD_SKYMAP_PIXELSHADER].Get();
    if(debug)
        pxShader = pixelShader[PIXELSHADER::FULLSCREENQUAD_SKYMAP_PIXELSHADER_DEBUG].Get();
    dcContext->PSSetShader(pxShader, nullptr, 0);

    dcContext->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
    dcContext->RSSetState(rasze);
    dcContext->OMSetDepthStencilState(depth, 1);
}



void ShaderManager::End(ID3D11DeviceContext* dcContext)
{
    dcContext->IASetInputLayout(nullptr);
    dcContext->VSSetShader(nullptr, nullptr, 0);
    dcContext->PSSetShader(nullptr, nullptr, 0);
    dcContext->GSSetShader(nullptr, nullptr, 0);
}


