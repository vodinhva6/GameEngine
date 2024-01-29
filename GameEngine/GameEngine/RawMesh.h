#pragma once
#ifndef RAWMESH_H
#define RAWMESH_H

#include <Skeleton.h>
#include <Material.h>
#include <wrl.h>
#include <memory>
//class RawVertex
//{
//public:
//    virtual ~RawVertex() {}
//  
//    VECTOR3 getPosition() { return position; }
//    VECTOR3 getNormal() { return normal; }
//    VECTOR4 getTangent() { return tangent; }
//    VECTOR2 getTexcoord() { return texcoord; }
//
//    virtual float* getboneWeights() { return nullptr; }
//    virtual uint32_t* getboneIndices() { return nullptr; }
//
//    void setPosition(const VECTOR3& value) { position = value; }
//    void setNormal(const VECTOR3& value) { normal = value; }
//    void setTangent(const VECTOR4& value) { tangent = value; }
//    void setTexcoord(const VECTOR3& value) { texcoord = value; }
//
//
//    template < class T>
//    void serialize(T& archive)
//    {
//        archive(position, normal, tangent, texcoord);
//    }
//protected:
//    VECTOR3 position = { 0,0,0 };
//    VECTOR3 normal = { 1,0,0 };
//    VECTOR4 tangent = { 1,0,0,0 };
//    VECTOR2 texcoord = { 0,0 };
//  
//    
//};
struct VertexBuff
{
    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 normal = {};
    DirectX::XMFLOAT4 tangent = {};
    DirectX::XMFLOAT2 texcoord = {};
    float boneWeights[MAX_BONE_INFLUENCES] = { 1, 0, 0, 0 };
    uint32_t boneIndices[MAX_BONE_INFLUENCES]{};
    template < class T>
        void serialize(T& archive)
        {
            archive(position, normal, tangent, texcoord, boneWeights, boneIndices);
        }
};
//class BoneVertex : public RawVertex
//{
//public:
//   
//    float* getboneWeights()    override { return boneWeights; }
//    uint32_t* getboneIndices() override { return boneIndices; }
//    template < class T>
//    void serialize(T& archive)
//    {
//        archive(cereal::base_class<RawVertex>(this), boneWeights, boneIndices);
//    }
//
//   
//    ~BoneVertex() override {}
//
//    float boneWeights[MAX_BONE_INFLUENCES] = { 1, 0, 0, 0 };
//       uint32_t boneIndices[MAX_BONE_INFLUENCES]{};
//};

//CEREAL_REGISTER_TYPE(BoneVertex);

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
    virtual void CreateCOM(ID3D11Device* device) {}
    std::vector<VertexBuff>& GetVertexList() { return boneVertices; }
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
            boundingBox, boneVertices, indices);
    }
protected:
    std::vector<VertexBuff> boneVertices;
};

class RawMesh : public BaseMesh
{
public:
    void CreateCOM(ID3D11Device* device) override;
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<BaseMesh>(this));
    }
};
CEREAL_REGISTER_TYPE(RawMesh);

class SkeletonMesh : public BaseMesh
{
public:
    void CreateCOM(ID3D11Device* device) override;
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<BaseMesh>(this), bindPose);
    }

    Skeleton bindPose;
   
   
};
CEREAL_REGISTER_TYPE(SkeletonMesh);



#endif // !RAWMESH_H