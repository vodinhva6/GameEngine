#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>
#include <MeshRaw.h>
class GeometricPrimitive : public StaticMesh
{
 
public:
    GeometricPrimitive(ID3D11Device* device);
    GeometricPrimitive() {}
    ~GeometricPrimitive() {}

    //void renderCube(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const VECTOR4& color);
    //void renderBoudingBox(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& color);
    //
    //void renderPointList(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const VECTOR4& color);
    //void renderPointStrip(ID3D11DeviceContext* immediate_context, DirectX::XMFLOAT4X4& world, const VECTOR4& color);

public:
    struct constants
    {
        DirectX::XMFLOAT4X4 world;
    };

protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

protected:
    //virtual void CreateCOMBuffers(ID3D11Device* device, Vertex* vertices, size_t vertex_count,
    //    uint32_t* indices, size_t index_count);
    //void create_com_buffers_line(ID3D11Device* device, Vertex* vertices, size_t vertex_count,
    //    uint32_t* indices, size_t index_count);
};

