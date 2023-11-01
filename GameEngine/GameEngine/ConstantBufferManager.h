#pragma once
#include "ConstantBuffer.h"
#include <vector>
class ConstantBufferManager
{
public:
    ConstantBufferManager() {}
    void Init(ID3D11Device* device);
    void setPSSlotNum(ID3D11DeviceContext* dcContext);
    void setVSSlotNum(ID3D11DeviceContext* dcContext);
    void setGSSlotNum(ID3D11DeviceContext* dcContext);
    void updateResource(ID3D11DeviceContext* dcContext);
    ~ConstantBufferManager();
private:
    std::vector<ConstantBuffer*> constantBufferList;


private:
    void CreateConstantBuffer(ID3D11Device* device);
};


