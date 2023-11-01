#include "FrameEffect.h"
#include "FrameBufferManager.h"
#include "GameEngine.h"
#include "ShaderManager.h"
#include "CameraManager.h"



void FrameEffectPSShadowMap::Setup(IDXGISwapChain* swapChain, ID3D11Device* device, const D3D11_VIEWPORT& viewPort, bool msaa, FrameBufferName name, bool shadow, bool depth)
{
	HRESULT hr = S_OK;

	renderTargetView.Reset();
	depthStencilView.Reset();
	shaderResourceView[0].Reset();
	shaderResourceView[1].Reset();
	this->name = name;
	viewport_ = viewPort;
	viewport_.Height = 1024 * 4;
	viewport_.Width = 1024 * 4;
	D3D11_TEXTURE2D_DESC texture2d_desc = {};
	texture2d_desc.Width = (UINT)viewport_.Width;
	texture2d_desc.Height = (UINT)viewport_.Height;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = cascadeCount;
	texture2d_desc.Format = DXGI_FORMAT_R32_TYPELESS;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;
	hr = device->CreateTexture2D(&texture2d_desc, 0, depthStencilBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = {};
	depth_stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depth_stencil_view_desc.Texture2DArray.FirstArraySlice = 0;
	depth_stencil_view_desc.Texture2DArray.ArraySize = static_cast<UINT>(cascadeCount);
	depth_stencil_view_desc.Texture2DArray.MipSlice = 0;
	depth_stencil_view_desc.Flags = 0;
	hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), &depth_stencil_view_desc, depthStencilView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
	shader_resource_view_desc.Format = DXGI_FORMAT_R32_FLOAT; // DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_R16_UNORM
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shader_resource_view_desc.Texture2DArray.ArraySize = static_cast<UINT>(cascadeCount);
	shader_resource_view_desc.Texture2DArray.MipLevels = 1;
	shader_resource_view_desc.Texture2DArray.FirstArraySlice = 0;
	shader_resource_view_desc.Texture2DArray.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(depthStencilBuffer.Get(), &shader_resource_view_desc, shaderResourceView[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

}

void FrameEffectPSShadowMap::DrawedOn(ID3D11DeviceContext* immediateContext)
{
	FrameBufferManager* frambufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
	ShaderManager* shaderManager = GetFrom<ShaderManager>(GameEngine::get()->getShaderManager());

	frambufferManager->Activate(immediateContext, this);
	shaderManager->BeginCastShadow(immediateContext);
	GameEngine::get()->CastShadow(FrameBufferName::MSAABUFFER1, DrawType::SkinnedMeshObject, cascadeCount);
	shaderManager->End(immediateContext);
	frambufferManager->Deactivate(immediateContext,this);

}

void FrameEffectPSShadowMap::DrawToDebug()
{
}

FrameEffectPSShadowMap::~FrameEffectPSShadowMap()
{
}
