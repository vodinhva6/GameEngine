#pragma once
#ifndef SAMPLERSTATEMANAGER_H
#define SAMPLERSTATEMANAGER_H
#include <d3d11.h>
#include <wrl.h>

#include "DataFormat.h"
class SamplerStateManager
{
public:
    SamplerStateManager() {}
    void Init(ID3D11Device* device);
    ID3D11SamplerState* getSamplerState(SamplerStates num);
    void setSamplerState(ID3D11DeviceContext* dcContext);
    ~SamplerStateManager() {}
private:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[MAX_SAMPLER_STATES];

};



#endif // !SAMPLERSTATEMANAGER_H

