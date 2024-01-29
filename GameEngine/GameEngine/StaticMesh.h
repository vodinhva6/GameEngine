#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include <fstream>
#include "DataFormat.h"
#include "ShaderManager.h"
#include "GeometricPrimitive.h"
HRESULT make_dummy_texture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view, DWORD value/*0xAABBGGRR*/, UINT dimension);
//class StaticMesh
//{
//public:
//    struct BoundingBox
//    {
//        VECTOR3 sizeMin;
//        VECTOR3 sizeMax;
//        VECTOR3  getSizeMin() { return sizeMin; }
//        VECTOR3  getSizeMax() { return sizeMax; }
//    };
//    BoundingBox boundingBox;
//    struct vertex
//    {
//        DirectX::XMFLOAT3 position;
//        DirectX::XMFLOAT3 normal;
//        DirectX::XMFLOAT2 texcoord;
//    };
//    struct constants
//    {
//        DirectX::XMFLOAT4X4 world;
//        //DirectX::XMFLOAT4 material_color;
//        DirectX::XMFLOAT4 Ka;
//        DirectX::XMFLOAT4 Kd;
//        DirectX::XMFLOAT4 Ks;
//    };
//    struct subset
//    {
//        std::wstring usemtl;
//        uint32_t index_start{ 0 }; // start position of index buffer
//        uint32_t index_count{ 0 }; // number of vertices (indices)
//    };
//    std::vector<subset> subsets;
//
//private:
//    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
//    Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
//
//    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
//    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
//    Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
//    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;
//
//    //std::wstring texture_filename;
//    //Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
//    struct material
//    {
//        std::wstring name;
//        DirectX::XMFLOAT4 Ka{ 0.2f, 0.2f, 0.2f, 1.0f };
//        DirectX::XMFLOAT4 Kd{ 0.8f, 0.8f, 0.8f, 1.0f };
//        DirectX::XMFLOAT4 Ks{ 1.0f, 1.0f, 1.0f, 1.0f };
//        std::wstring texture_filename[2];
//        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view[2];
//    };
//    std::vector<material> materials;
//
//    
//    
//    
//
//
//
//public:
//    StaticMesh(ID3D11Device* device, const wchar_t* obj_filename, ShaderData);
//    virtual ~StaticMesh() = default;
//
//    void render(ID3D11DeviceContext * immediate_context,
//        DirectX::XMFLOAT4X4 & world, const DirectX::XMFLOAT4 & material_color);
//    bool BoundingBox;
//protected:
//    void create_com_buffers(ID3D11Device * device, vertex * vertices, size_t vertex_count,
//        uint32_t * indices, size_t index_count);
//};

