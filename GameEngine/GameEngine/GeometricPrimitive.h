#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>
#include "DataFormat.h"
class GeometricPrimitive
{
   
public:
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
    };
    struct constants
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 material_color;

    };

    


protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;
    
 
public:
    
        

    GeometricPrimitive(ID3D11Device* device, const VECTOR3& sizeMin, const VECTOR3& sizeMax, ShaderData shaderData);
    GeometricPrimitive() {}
    virtual ~GeometricPrimitive() = default;

    void renderCube(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const VECTOR4& color);
    void renderBoudingBox(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& color);

    void renderPointList(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const VECTOR4& color);
    void renderPointStrip(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const VECTOR4& color);

protected:
    virtual void createCOMBuffers(ID3D11Device* device, Vertex* vertices, size_t vertex_count,
        uint32_t* indices, size_t index_count);
    void create_com_buffers_line(ID3D11Device* device, Vertex* vertices, size_t vertex_count,
        uint32_t* indices, size_t index_count);
};

