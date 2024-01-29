#pragma once
#ifndef SKINNEDMESH_H
#define SKINNEDMESH_H

#include <vector>
#include <unordered_map>
#include <Animator.h>
#include "DataFormat.h"
#include "Material.h"
#include "MeshRaw.h"

class SkinnedMesh
{
public:
 

    struct Constants
    {
        DirectX::XMFLOAT4X4 world = 
        {
             1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 
        };
        DirectX::XMFLOAT4X4 bone_transforms[MAX_BONES]
        {
            { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } 
        };

        VECTOR4 Ka = {1,0,0,1};
        VECTOR4 Kd = {1,0,0,1};
        VECTOR4 Ks = {1,0,0,1};
        float occlusionStrength;
        float smoothness;
        float metallicfactor;
        float dummyObjConstant;
    };
    VECTOR3 VertexMinMaxInfor[2]
    {
        { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX },
        { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
    };
protected:
    std::vector<SkeletonMesh> meshes;
protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    std::string fileLocal;


    const DirectX::XMFLOAT4X4 coordinate_system_transforms[4]
    {
        { -1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1, 0,
           0, 0, 0, 1 },


        { 1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1 },   // 1:LHS Y-UP


        { -1, 0, 0, 0,
           0, 0,-1, 0,
           0, 1, 0, 0,
           0, 0, 0, 1 }, // 2:RHS Z-UP

        { 1, 0, 0, 0,
          0, 0, 1, 0,
          0, 1, 0, 0,
          0, 0, 0, 1 },   // 3:LHS Z-UP
    };
    const float scale_factor = 1.0f;
public:
    SkinnedMesh() {}
    
    void CreateComObjects(ID3D11Device*);
    void Render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4 & world, 
        const std::unordered_map<int64_t,std::shared_ptr<Material>>& materialList,
        const VECTOR4& material_color,
        Animation::Keyframe* keyFrame = nullptr);
    void ShadowCaster(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world,
        const std::unordered_map<int64_t, std::shared_ptr<Material>>& materialList,
        const VECTOR4& material_color, UINT numIndex, Animation::Keyframe* keyFrame = nullptr);
    void CreateNewCereal();
    bool IsCanLoadCereal(std::string local);
    virtual ~SkinnedMesh();

public:
    std::string& getLocalPath() { return fileLocal; }
    std::vector<SkeletonMesh>& getMeshRawList() { return meshes; }
    DirectX::XMFLOAT4X4& getDefaultTransform() { return defaultTransform; }


    void SetDefaultTransform(DirectX::XMFLOAT4X4& tfDefault);
    void SetLocalPath(std::string local) { fileLocal = local; }
public:
    template < class T>
    void serialize(T& archive)
    {
        archive(meshes, fileLocal, defaultTransform);
    }


private:
    void UpdateVertexMaxMinInfor();

private:
    DirectX::XMFLOAT4X4 defaultTransform =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

#endif // !SKINNEDMESH_H