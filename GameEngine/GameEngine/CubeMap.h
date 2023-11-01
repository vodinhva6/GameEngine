#pragma once
#include "GeometricPrimitive.h"
#include <wrl.h>
#include <memory>
#include <filesystem>
//class SkyBox : public GeometricPrimitive
//{
//public:
//    SkyBox(ID3D11Device* device, float farD, std::filesystem::path textureLocal);
//    void Render(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world);
//    ~SkyBox();
//public:
//    VECTOR4& getColor() { return color; }
//
//    void setColor(const VECTOR4& color) { this->color = color; }
//private:
//    void createCOMBuffersSkyBox(ID3D11Device* device, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
//
//
//private:
//    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyboxMap[6];
//    VECTOR4 color;
//
//};

