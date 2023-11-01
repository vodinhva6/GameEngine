#include "ConstantBufferManager.h"
#include "LightConstant.h"
#include "ConstantBloom.h"
void ConstantBufferManager::Init(ID3D11Device* device)
{
    constantBufferList.push_back(new SceneBuffer);
    constantBufferList.push_back(new LightBuffer);
    constantBufferList.push_back(new ColorFilterBuffer);
    constantBufferList.push_back(new FogBuffer);
    constantBufferList.push_back(new BloomBuffer);

    CreateConstantBuffer(device);
}

void ConstantBufferManager::setPSSlotNum(ID3D11DeviceContext* dcContext)
{
    for (UINT32 i = 0; i < constantBufferList.size(); i++)
    {
        int slot = constantBufferList[i]->getPSSlotNum();
        if (slot > 0)
            dcContext->PSSetConstantBuffers(slot, 1, constantBufferList[i]->getAddrBuffer());
    }

}

void ConstantBufferManager::setVSSlotNum(ID3D11DeviceContext* dcContext)
{
    for (UINT32 i = 0; i < constantBufferList.size(); i++)
    {
        int slot = constantBufferList[i]->getVSSlotNum();
        if (slot > 0)
            dcContext->VSSetConstantBuffers(slot, 1, constantBufferList[i]->getAddrBuffer());
    }
}

void ConstantBufferManager::setGSSlotNum(ID3D11DeviceContext* dcContext)
{
    for (UINT32 i = 0; i < constantBufferList.size(); i++)
    {
        int slot = constantBufferList[i]->getGSSlotNum();
        if (slot > 0)
            dcContext->GSSetConstantBuffers(slot, 1, constantBufferList[i]->getAddrBuffer());
    }
}

void ConstantBufferManager::updateResource(ID3D11DeviceContext* dcContext)
{
    for (UINT32 i = 0; i < constantBufferList.size(); i++)
    {
        constantBufferList[i]->UpdateBuffer(dcContext);
    }
}

ConstantBufferManager::~ConstantBufferManager()
{
    for (UINT32 i = 0; i < constantBufferList.size(); i++)
    {
        SafeDelete(constantBufferList[i]);
    }
}

void ConstantBufferManager::CreateConstantBuffer(ID3D11Device* device)
{
    for (UINT32 i = 0; i < constantBufferList.size(); i++)
    {
        constantBufferList[i]->CreateBuffer(device);
    }
}
