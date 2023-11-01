#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "DataFormat.h"
class ConstantBuffer
{
public:
    virtual void CreateBuffer(ID3D11Device* device) = 0;
    virtual void UpdateBuffer(ID3D11DeviceContext* dcContext) = 0;
    virtual ~ConstantBuffer() {};
    int getPSSlotNum() { return PSSlotNum; }
    int getVSSlotNum() { return VSSlotNum; }
    int getGSSlotNum() { return GSSlotNum; }
    ID3D11Buffer** getAddrBuffer() { return constantBuffer.GetAddressOf(); }
protected:
    int PSSlotNum;
    int VSSlotNum;
    int GSSlotNum = -1;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
};

class SceneBuffer: public ConstantBuffer
{
public:
    void CreateBuffer(ID3D11Device* device) override;
    void UpdateBuffer(ID3D11DeviceContext* dcContext) override;
    ~SceneBuffer() override {}
private:
    struct SceneConstant
    {
        DirectX::XMFLOAT4X4 viewProjectionNow;
        DirectX::XMFLOAT4X4 debugViewProjection;
        DirectX::XMFLOAT4X4 debugViewProjectionInvers;
        DirectX::XMFLOAT4X4 viewProjectionNowInvers;
        DirectX::XMFLOAT4X4 viewProjectionOld;
        DirectX::XMFLOAT4X4 projectionNowInvers;
        VECTOR4 gameCameraPosition;
        VECTOR4 debugCameraPosition;
        int motisonBlurSample;
        DirectX::XMFLOAT3 dummy;
    };
    SceneConstant sceneConstant;
    
};



class ColorFilterBuffer : public ConstantBuffer
{
public:
    void CreateBuffer(ID3D11Device* device) override;
    void UpdateBuffer(ID3D11DeviceContext* dcContext) override;
    ~ColorFilterBuffer() override {}

private:
    struct ColorFilter
    {
        float hueShift;  
        float saturation;
        float brightness;
        float gamma;
    };
    ColorFilter colorFilerConstant;

};



class FogBuffer : public ConstantBuffer
{
public:
    void CreateBuffer(ID3D11Device* device) override;
    void UpdateBuffer(ID3D11DeviceContext* dcContext) override;
    ~FogBuffer() override {}

private:
    struct FogConstant
    {
        VECTOR4 fogRange;
        VECTOR4 fogColor;
    };
    FogConstant fogConstant;

};