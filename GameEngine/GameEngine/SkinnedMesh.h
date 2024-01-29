#pragma once
#ifndef SKINNEDMESH_H
#define SKINNEDMESH_H

#include "Mesh.h"

class SkinnedMesh : public Meshes
{
public:
    SkinnedMesh() {}

    void CreateComObjects(ID3D11Device*);
    void Render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world,
        const std::unordered_map<int64_t, std::shared_ptr<Material>>& materialList,
        const VECTOR4& material_color,
        Animation::Keyframe* keyFrame = nullptr);
    void ShadowCaster(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world,
        const std::unordered_map<int64_t, std::shared_ptr<Material>>& materialList,
        const VECTOR4& material_color, UINT numIndex, Animation::Keyframe* keyFrame = nullptr);
    void CreateNewCereal() override;
    bool IsCanLoadCereal(std::string local) override;
    virtual ~SkinnedMesh();

public:
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<Meshes>(this));
    }

private:
    void UpdateVertexMaxMinInfor() override;

};
#endif // !SKINNEDMESH_H