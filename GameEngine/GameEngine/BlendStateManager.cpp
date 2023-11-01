#include "BlendStateManager.h"
#include "Misc.h"

void BlendStateManager::Init(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blendDesc, blendStates[BlendStates::Alpha].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	
	blendDesc.AlphaToCoverageEnable = TRUE;
	hr = device->CreateBlendState(&blendDesc, blendStates[BlendStates::AlphaBlendAlphaCoverage].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT;
	hr = device->CreateBlendState(&blendDesc, blendStates[BlendStates::RevSubtract].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT;
	hr = device->CreateBlendState(&blendDesc, blendStates[BlendStates::Additively].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	
}

ID3D11BlendState* BlendStateManager::getBlendState(BlendStates num)
{
    return blendStates[(int)num].Get();
}
