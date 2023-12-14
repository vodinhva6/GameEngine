#pragma once

#ifndef MESH_H
#define MESH_H
#include <vector>
#include <unordered_map>
#include <Animator.h>
#include "DataFormat.h"
#include "Material.h"
#include "MeshRaw.h"

class Mesh
{
public:
    Mesh();

    DirectX::XMFLOAT4X4& getDefaultTransform() { return defaultTransform; }
    std::string& getLocalPath() { return fileLocal; }
    void SetLocalPath(std::string local) { fileLocal = local; }
    void SetDefaultTransform(DirectX::XMFLOAT4X4& tfDefault);
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

    virtual void UpdateVertexMaxMinInfor() {}
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

    std::string fileLocal;
    VECTOR3 VertexMinMaxInfor[2];
    DirectX::XMFLOAT4X4 defaultTransform;


};




#endif // !MESH_H
