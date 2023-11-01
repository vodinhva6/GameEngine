#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "DataFormat.h"
class BlendStateManager
{
public:
    BlendStateManager() {}
    void Init(ID3D11Device* device);
    ID3D11BlendState* getBlendState(BlendStates num);
    ~BlendStateManager() {}
private:
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[MAX_BLEND_STATES];
    
};

