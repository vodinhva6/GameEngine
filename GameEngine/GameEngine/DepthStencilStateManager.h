#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "DataFormat.h"
class DepthStencilStateManager
{
public:
    DepthStencilStateManager() {}
    void Init(ID3D11Device* device);
    ID3D11DepthStencilState* getDepthStencill(DepthStencilStates num);
    ~DepthStencilStateManager() {}

private:
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[MAX_BLEND_STATES];
};

