#include "ConstantBuffer.h"

class BloomBuffer : public ConstantBuffer
{
public:
    void CreateBuffer(ID3D11Device* device) override;
    void UpdateBuffer(ID3D11DeviceContext* dcContext) override;
    ~BloomBuffer() override {}

private:
    struct Bloom
    {
        float bloomExtractionThreshold;
        float bloomIntensity;
        VECTOR2 dummy;
    };
    Bloom bloomConstant;

};