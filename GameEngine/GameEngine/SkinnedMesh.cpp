#include "SkinnedMesh.h"
#include "Misc.h"
void SkinnedMesh::CreateComObjects(ID3D11Device* device)
{
    HRESULT hr = S_OK;

   

    for (auto& mesh : meshes)
    {
        mesh->CreateCOM(device);
    }
        //
    
    UpdateVertexMaxMinInfor();
    Meshes::CreateComObjects(device);

}


void SkinnedMesh::Render(ID3D11DeviceContext* immediate_context,
    const DirectX::XMFLOAT4X4& world, 
    const std::unordered_map<int64_t, std::shared_ptr<Material>>& materialList,
    const VECTOR4& material_color,
    Animation::Keyframe* keyFrame)
{
    
    for (auto& it : meshes)
    {
        std::shared_ptr<SkeletonMesh> mesh = std::dynamic_pointer_cast<SkeletonMesh>(it);
        uint32_t stride{ sizeof(VertexBuff) };
        uint32_t offset{ 0 };
        immediate_context->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
        immediate_context->IASetIndexBuffer(mesh->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        Constants data;


        if (keyFrame)
        {
            Animation::Keyframe::Node& mesh_node{ keyFrame->nodes.at(mesh->nodeIndex) };

            DirectX::XMStoreFloat4x4(&data.world, DirectX::XMLoadFloat4x4(&mesh_node.global_transform) * DirectX::XMLoadFloat4x4(&defaultTransform) * XMLoadFloat4x4(&world));

            const size_t bone_count{ mesh->bindPose.bones.size() };
            _ASSERT_EXPR(bone_count < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");

            for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
            {
                Skeleton::Bone& bone{ mesh->bindPose.bones.at(bone_index) };
                Animation::Keyframe::Node& bone_node{ keyFrame->nodes.at(bone.nodeIndex) };
                DirectX::XMStoreFloat4x4(&data.bone_transforms[bone_index],
                    DirectX::XMLoadFloat4x4(&bone.offsetTransform) *
                    DirectX::XMLoadFloat4x4(&bone_node.global_transform) *
                    DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mesh->defaultGlobalTransform))
                );

            }
        }
        else
        {
            DirectX::XMStoreFloat4x4(&data.world, DirectX::XMLoadFloat4x4(&mesh->defaultGlobalTransform) *
                DirectX::XMLoadFloat4x4(&defaultTransform) * XMLoadFloat4x4(&world));
            for (size_t bone_index = 0; bone_index < MAX_BONES; ++bone_index)
            {
                data.bone_transforms[bone_index] = { 1, 0, 0, 0,
                                                     0, 1, 0, 0,
                                                     0, 0, 1, 0,
                                                     0, 0, 0, 1 };

            }
        }
        for (const Subset& subset : mesh->subsets)
        {
            auto it = materialList.find(subset.materialUniqueId);
            std::shared_ptr<Material> material = materialList.begin()->second;
            if (it != materialList.end())
                material = it->second;
            data.Ka = material->getKa();
            data.Kd = material->getKd();
            data.Ks = material->getKs();
            data.smoothness = material->getSmoothness();
            data.occlusionStrength = material->getOcclusionStrength();
            data.metallicfactor = material->getMetallicfactor();

            immediate_context->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
            immediate_context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
            immediate_context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

            immediate_context->PSSetShaderResources(0, 1, material->getTextureShaderResource(0).GetAddressOf());
            immediate_context->PSSetShaderResources(1, 1, material->getTextureShaderResource(1).GetAddressOf());
            immediate_context->PSSetShaderResources(2, 1, material->getTextureShaderResource(2).GetAddressOf());
            immediate_context->PSSetShaderResources(3, 1, material->getTextureShaderResource(3).GetAddressOf());
            immediate_context->PSSetShaderResources(4, 1, material->getTextureShaderResource(4).GetAddressOf());

            immediate_context->DrawIndexed(subset.indexCount, subset.startIndexLocation, 0);
        }
    }
}

void SkinnedMesh::ShadowCaster(ID3D11DeviceContext* immediate_context, 
    const DirectX::XMFLOAT4X4& world, const std::unordered_map<int64_t, std::shared_ptr<Material>>& materialList,
    const VECTOR4& material_color, UINT numIndex, Animation::Keyframe* keyFrame)
{
    for (auto& it : meshes)
    {
        std::shared_ptr<SkeletonMesh> mesh = std::dynamic_pointer_cast<SkeletonMesh>(it);
        uint32_t stride{ sizeof(VertexBuff) };
        uint32_t offset{ 0 };
        immediate_context->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
        immediate_context->IASetIndexBuffer(mesh->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


        Constants data;
        if (keyFrame)
        {
            Animation::Keyframe::Node& mesh_node{ keyFrame->nodes.at(mesh->nodeIndex) };

            DirectX::XMStoreFloat4x4(&data.world, DirectX::XMLoadFloat4x4(&mesh_node.global_transform) * DirectX::XMLoadFloat4x4(&defaultTransform) * XMLoadFloat4x4(&world));

            const size_t bone_count{ mesh->bindPose.bones.size() };
            _ASSERT_EXPR(bone_count < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");

            for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
            {
                Skeleton::Bone& bone{ mesh->bindPose.bones.at(bone_index) };
                Animation::Keyframe::Node& bone_node{ keyFrame->nodes.at(bone.nodeIndex) };
                DirectX::XMStoreFloat4x4(&data.bone_transforms[bone_index],
                    DirectX::XMLoadFloat4x4(&bone.offsetTransform) *
                    DirectX::XMLoadFloat4x4(&bone_node.global_transform) *
                    DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mesh->defaultGlobalTransform))
                );

            }
        }
        else
        {
            DirectX::XMStoreFloat4x4(&data.world, DirectX::XMLoadFloat4x4(&mesh->defaultGlobalTransform) * 
                DirectX::XMLoadFloat4x4(&defaultTransform) * XMLoadFloat4x4(&world));
            for (size_t bone_index = 0; bone_index < MAX_BONES; ++bone_index)
            {
                data.bone_transforms[bone_index] = { 1, 0, 0, 0,
                                                     0, 1, 0, 0,
                                                     0, 0, 1, 0,
                                                     0, 0, 0, 1 };

            }
        }
        for (const Subset& subset : mesh->subsets)
        {
            auto it = materialList.find(subset.materialUniqueId);
            std::shared_ptr<Material> material = materialList.begin()->second;
            if (it != materialList.end())
                material = it->second;
            immediate_context->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
            immediate_context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
            immediate_context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

            immediate_context->PSSetShaderResources(0, 1, material->getTextureShaderResource(0).GetAddressOf());

            immediate_context->DrawIndexedInstanced(subset.indexCount, numIndex, subset.startIndexLocation, 0, 0);
        }
    }
}


SkinnedMesh::~SkinnedMesh()
{

}


void SkinnedMesh::UpdateVertexMaxMinInfor()
{
    for (auto& mesh : meshes)
    {
        VertexMinMaxInfor[0].x = std::min<float>(VertexMinMaxInfor[0].x, mesh->boundingBox[0].x);
        VertexMinMaxInfor[0].y = std::min<float>(VertexMinMaxInfor[0].y, mesh->boundingBox[0].y);
        VertexMinMaxInfor[0].z = std::min<float>(VertexMinMaxInfor[0].z, mesh->boundingBox[0].z);
        VertexMinMaxInfor[1].x = std::max<float>(VertexMinMaxInfor[1].x, mesh->boundingBox[1].x);
        VertexMinMaxInfor[1].y = std::max<float>(VertexMinMaxInfor[1].y, mesh->boundingBox[1].y);
        VertexMinMaxInfor[1].z = std::max<float>(VertexMinMaxInfor[1].z, mesh->boundingBox[1].z);
    }
    DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&VertexMinMaxInfor[0]);
    DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&defaultTransform);
    DirectX::XMStoreFloat3(&VertexMinMaxInfor[0], DirectX::XMVector3TransformCoord(V, M));
    V = DirectX::XMLoadFloat3(&VertexMinMaxInfor[1]);
    DirectX::XMStoreFloat3(&VertexMinMaxInfor[1], DirectX::XMVector3TransformCoord(V, M));
}



void SkinnedMesh::CreateNewCereal()
{
    //std::filesystem::path cerealFileName(fileLocal);
    //cerealFileName.replace_extension("mes");
    //std::ofstream ofs(cerealFileName.c_str(), std::ios::binary);
    //cereal::BinaryOutputArchive serialization(ofs);
    //serialization(meshes, fileLocal, defaultTransform);
}

bool SkinnedMesh::IsCanLoadCereal(std::string local)
{
    fileLocal = local;
    std::filesystem::path cereal_filename(fileLocal);
    cereal_filename.replace_extension("mes");
    if (std::filesystem::exists(cereal_filename.c_str()))
    {
        //std::ifstream ifs(cereal_filename.c_str(), std::ios::binary);
        //cereal::BinaryInputArchive deserialization(ifs);
        //deserialization(meshes, fileLocal, defaultTransform);
        return true;
    }
  
    return false;
}
