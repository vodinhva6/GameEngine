#pragma once

#ifndef MESH_H
#define MESH_H
#include <vector>
#include <unordered_map>
#include <Animator.h>
#include <DataFormat.h>
#include <Material.h>
#include <RawMesh.h>

class Meshes
{
public:
    Meshes();

    virtual void CreateComObjects(ID3D11Device*);
    virtual void Render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world,
        const std::unordered_map<int64_t, std::shared_ptr<Material>>& materialList,
        const VECTOR4& material_color,
        Animation::Keyframe* keyFrame = nullptr) = 0;
    virtual void ShadowCaster(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world,
        const std::unordered_map<int64_t, std::shared_ptr<Material>>& materialList,
        const VECTOR4& material_color, UINT numIndex, Animation::Keyframe* keyFrame = nullptr) = 0;
    virtual bool IsCanLoadCereal(std::string local) = 0;
    virtual void CreateNewCereal() = 0;

    void SetLocalPath(std::string local) { fileLocal = local; }
    VECTOR3 getVertexMin() { return VertexMinMaxInfor[1]; }
    VECTOR3 getVertexMax() { return VertexMinMaxInfor[0]; }
    DirectX::XMFLOAT4X4& getDefaultTransform() { return defaultTransform; }
    std::string& getLocalPath() { return fileLocal; }
    std::vector<std::shared_ptr<BaseMesh>>& getMeshList() { return meshes; }
    void SetDefaultTransform(DirectX::XMFLOAT4X4& tfDefault);

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

        VECTOR4 Ka = { 1,1,1,1 };
        VECTOR4 Kd = { 1,1,1,1 };
        VECTOR4 Ks = { 1,1,1,1 };
        float occlusionStrength;
        float smoothness;
        float metallicfactor;
        float dummyObjConstant;
    };


public:
    template < class T>
    void serialize(T& archive)
    {
        archive(fileLocal, defaultTransform);
    }


protected:

    virtual void UpdateVertexMaxMinInfor() = 0;

    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

    std::vector<std::shared_ptr<BaseMesh>> meshes;
    std::string fileLocal;


    VECTOR3 VertexMinMaxInfor[2];
    DirectX::XMFLOAT4X4 defaultTransform;


};




#endif // !MESH_H
