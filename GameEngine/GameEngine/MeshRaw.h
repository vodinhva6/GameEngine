#pragma once
#include "Skeleton.h"
#include "Material.h"
#include <wrl.h>
#include <memory>
class RawVertex
{
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(position, normal, tangent, texcoord);
    }

    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 normal = { 1,0,0 };
    DirectX::XMFLOAT4 tangent = { 1,0,0,0 };
    DirectX::XMFLOAT2 texcoord = { 0,0 };
  
   
};

class BoneVertex : public RawVertex
{
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<RawVertex>(this), boneWeights, boneIndices);
    }
    float boneWeights[MAX_BONE_INFLUENCES]{ 1, 0, 0, 0 };
    uint32_t boneIndices[MAX_BONE_INFLUENCES]{};
};

class Subset
{
public:
    uint64_t materialUniqueId{ 0 };
    uint32_t startIndexLocation{ 0 };
    uint32_t indexCount{ 0 };
    template < class T>
    void serialize(T& archive)
    {
        archive(materialUniqueId, startIndexLocation, indexCount);
    }
};

class MeshRaw
{
public:
    virtual void CreateCOM(ID3D11Device* device) = 0;
public:
    DirectX::XMFLOAT3 boundingBox[2]
    {
        { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX },
        { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
    };
    uint64_t uniqueId{ 0 };
    std::string name;
    int64_t nodeIndex{ 0 };

   
    std::vector<uint32_t> indices;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    
    std::vector<Subset> subsets;
   

    DirectX::XMFLOAT4X4 defaultGlobalTransform{   1, 0, 0, 0,
                                                  0, 1, 0, 0,
                                                  0, 0, 1, 0,
                                                  0, 0, 0, 1  };

    template < class T>
    void serialize(T& archive)
    {
        archive(uniqueId, name, nodeIndex, subsets, defaultGlobalTransform,
            boundingBox, indices);
    }
    
};

class StaticMesh : public MeshRaw
{
public:
    void CreateCOM(ID3D11Device* device) override;
    std::vector<RawVertex> vertices;
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<MeshRaw>(this), vertices);
    }

};


class SkeletonMesh : public MeshRaw
{
public:
    void CreateCOM(ID3D11Device* device) override;
    std::vector<BoneVertex> vertices;
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<MeshRaw>(this), bindPose, vertices);
    }
    Skeleton bindPose;

};