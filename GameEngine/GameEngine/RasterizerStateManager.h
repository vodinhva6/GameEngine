#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "DataFormat.h"
class RasterizerStateManager
{
public:
    RasterizerStateManager() {}
    void Init(ID3D11Device* device);
    ID3D11RasterizerState* getRasterizerState(RasterizerStates num);
    ~RasterizerStateManager() {}

private:
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState[MAX_RASTERIZER_STATES];
};

