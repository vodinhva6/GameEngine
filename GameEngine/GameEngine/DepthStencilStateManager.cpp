#include "DepthStencilStateManager.h"
#include "Misc.h"

void DepthStencilStateManager::Init(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[(int)DepthStencilStates::DepthTestOn_DepthWriteOn].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));									 
	depthStencilDesc.DepthEnable = FALSE;										 
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				 
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;					 
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[(int)DepthStencilStates::DepthTestOff_DepthWriteOff].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));									 
	depthStencilDesc.DepthEnable = TRUE;										 
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				 
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;					 
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[(int)DepthStencilStates::DepthTestOn_DepthWriteOff].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));									
	depthStencilDesc.DepthEnable = FALSE;										
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;				
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;					
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[(int)DepthStencilStates::DepthTestOff_DepthWriteOn].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	
	
}

ID3D11DepthStencilState* DepthStencilStateManager::getDepthStencill(DepthStencilStates num)
{
	return depthStencilStates[(int)num].Get();
}
