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
    float boneWeights[MAX_BONE_INFLUENCES] = { 1, 0, 0, 0 };
    uint32_t boneIndices[MAX_BONE_INFLUENCES]{};
};

class Subset
{
public:
    uint64_t materialUniqueId = 0;
    uint32_t startIndexLocation = 0;
    uint32_t indexCount = 0;
    template < class T>
    void serialize(T& archive)
    {
        archive(materialUniqueId, startIndexLocation, indexCount);
    }
};

class BaseMesh
{
public:
    BaseMesh();
    virtual void CreateCOM(ID3D11Device* device) = 0;
public:
    DirectX::XMFLOAT3 boundingBox[2];
    DirectX::XMFLOAT4X4 defaultGlobalTransform;

    int64_t nodeIndex;
    uint64_t uniqueId;

    std::string name;
    std::vector<uint32_t> indices;
    std::vector<Subset> subsets;


    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    
   
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(uniqueId, name, nodeIndex, subsets, defaultGlobalTransform,
            boundingBox, indices);
    }
    
};

class RawMesh : public BaseMesh
{
public:
    void CreateCOM(ID3D11Device* device) override;
    std::vector<RawVertex> vertices;
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<BaseMesh>(this), vertices);
    }

};


class SkeletonMesh : public BaseMesh
{
public:
    void CreateCOM(ID3D11Device* device) override;
    std::vector<BoneVertex> vertices;
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<BaseMesh>(this), bindPose, vertices);
    }
    Skeleton bindPose;

};