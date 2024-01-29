#pragma once
#include "Skeleton.h"
#include "Material.h"
#include <wrl.h>
#include <memory>
class MeshRawVertex
{
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(position, normal, tangent, texcoord, bone_weights, bone_indices);
    }

    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 normal = { 1,0,0 };
    DirectX::XMFLOAT4 tangent = { 1,0,0,0 };
    DirectX::XMFLOAT2 texcoord = { 0,0 };
    float bone_weights[MAX_BONE_INFLUENCES]{ 1, 0, 0, 0 };
    uint32_t bone_indices[MAX_BONE_INFLUENCES]{};
   
};

class Subset
{
public:
    uint64_t material_unique_id{ 0 };
    uint32_t start_index_location{ 0 };
    uint32_t index_count{ 0 };
    template < class T>
    void serialize(T& archive)
    {
        archive(material_unique_id, start_index_location, index_count);
    }
};

class MeshRaw
{
public:
    DirectX::XMFLOAT3 bounding_box[2]
    {
        { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX },
        { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
    };
    uint64_t unique_id{ 0 };
    std::string name;
    int64_t node_index{ 0 };

    std::vector<MeshRawVertex> vertices;
    std::vector<uint32_t> indices;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
    
    std::vector<Subset> subsets;
    Skeleton bind_pose;
    DirectX::XMFLOAT4X4 default_global_transform{ 1, 0, 0, 0,
                                                  0, 1, 0, 0,
                                                  0, 0, 1, 0,
                                                  0, 0, 0, 1 };

    template < class T>
    void serialize(T& archive)
    {
        archive(unique_id, name, node_index, subsets, default_global_transform,
            bind_pose, bounding_box, vertices, indices);
    }
    
};

