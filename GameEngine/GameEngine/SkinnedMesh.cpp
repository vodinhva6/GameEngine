#include "SkinnedMesh.h"
#include "Misc.h"
void SkinnedMesh::CreateComObjects(ID3D11Device* device)
{
    HRESULT hr = S_OK;
    for (MeshRaw& mesh : meshes)
    {
        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC buffer_desc{};
        D3D11_SUBRESOURCE_DATA subresource_data{};
        buffer_desc.ByteWidth = static_cast<UINT>(sizeof(MeshRawVertex) * mesh.vertices.size());
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;
        subresource_data.pSysMem = mesh.vertices.data();
        subresource_data.SysMemPitch = 0;
        subresource_data.SysMemSlicePitch = 0;
        hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.vertex_buffer.ReleaseAndGetAddressOf());

        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * mesh.indices.size());
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        subresource_data.pSysMem = mesh.indices.data();
        hr = device->CreateBuffer(&buffer_desc, &subresource_data,
            mesh.index_buffer.ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
      
#if 0
        mesh.vertices.clear();
        mesh.indices.clear();
#endif
    }
    UpdateVertexMaxMinInfor();
    


    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


}


void SkinnedMesh::Render(ID3D11DeviceContext* immediate_context,
    const DirectX::XMFLOAT4X4& world, 
    const std::unordered_map<int64_t, std::shared_ptr<Material>>& materialList,
    const VECTOR4& material_color,
    Animation::Keyframe* keyFrame)
{
    
    for (MeshRaw& mesh : meshes)
    {
        uint32_t stride{ sizeof(MeshRawVertex) };
        uint32_t offset{ 0 };
        immediate_context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
        immediate_context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        constants data;


        if (keyFrame)
        {
            Animation::Keyframe::Node& mesh_node{ keyFrame->nodes.at(mesh.node_index) };

            DirectX::XMStoreFloat4x4(&data.world, DirectX::XMLoadFloat4x4(&mesh_node.global_transform) * DirectX::XMLoadFloat4x4(&defaultTransform) * XMLoadFloat4x4(&world));

            const size_t bone_count{ mesh.bind_pose.bones.size() };
            _ASSERT_EXPR(bone_count < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");

            for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
            {
                Skeleton::Bone& bone{ mesh.bind_pose.bones.at(bone_index) };
                Animation::Keyframe::Node& bone_node{ keyFrame->nodes.at(bone.node_index) };
                DirectX::XMStoreFloat4x4(&data.bone_transforms[bone_index],
                    DirectX::XMLoadFloat4x4(&bone.offset_transform) *
                    DirectX::XMLoadFloat4x4(&bone_node.global_transform) *
                    DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mesh.default_global_transform))
                );

            }
        }
        else
        {
            XMStoreFloat4x4(&data.world, XMLoadFloat4x4(&mesh.default_global_transform) * DirectX::XMLoadFloat4x4(&defaultTransform) * XMLoadFloat4x4(&world));
            for (size_t bone_index = 0; bone_index < MAX_BONES; ++bone_index)
            {
                data.bone_transforms[bone_index] = { 1, 0, 0, 0,
                                                     0, 1, 0, 0,
                                                     0, 0, 1, 0,
                                                     0, 0, 0, 1 };

            }
        }
        for (const Subset& subset : mesh.subsets)
        {
            auto it = materialList.find(subset.material_unique_id);
            std::shared_ptr<Material> material = materialList.begin()->second;
            if (it != materialList.end())
                material = it->second;
            data.Ka = material->getKa();
            data.Kd = material->getKd();
            data.Ks = material->getKs();
            data.smoothness = material->getSmoothness();
            data.occlusionStrength = material->getOcclusionStrength();
            data.metallicfactor = material->getMetallicfactor();

            immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
            immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
            immediate_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

            immediate_context->PSSetShaderResources(0, 1, material->getTextureShaderResource(0).GetAddressOf());
            immediate_context->PSSetShaderResources(1, 1, material->getTextureShaderResource(1).GetAddressOf());
            immediate_context->PSSetShaderResources(2, 1, material->getTextureShaderResource(2).GetAddressOf());
            immediate_context->PSSetShaderResources(3, 1, material->getTextureShaderResource(3).GetAddressOf());
            immediate_context->PSSetShaderResources(4, 1, material->getTextureShaderResource(4).GetAddressOf());

            immediate_context->DrawIndexed(subset.index_count, subset.start_index_location, 0);
        }
    }
}

void SkinnedMesh::ShadowCaster(ID3D11DeviceContext* immediate_context, 
    const DirectX::XMFLOAT4X4& world, const std::unordered_map<int64_t, std::shared_ptr<Material>>& materialList,
    const VECTOR4& material_color, UINT numIndex, Animation::Keyframe* keyFrame)
{
    for (MeshRaw& mesh : meshes)
    {
        uint32_t stride{ sizeof(MeshRawVertex) };
        uint32_t offset{ 0 };
        immediate_context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
        immediate_context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


        constants data;
        if (keyFrame)
        {
            Animation::Keyframe::Node& mesh_node{ keyFrame->nodes.at(mesh.node_index) };

            DirectX::XMStoreFloat4x4(&data.world, DirectX::XMLoadFloat4x4(&mesh_node.global_transform) * DirectX::XMLoadFloat4x4(&defaultTransform) * XMLoadFloat4x4(&world));

            const size_t bone_count{ mesh.bind_pose.bones.size() };
            _ASSERT_EXPR(bone_count < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");

            for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
            {
                Skeleton::Bone& bone{ mesh.bind_pose.bones.at(bone_index) };
                Animation::Keyframe::Node& bone_node{ keyFrame->nodes.at(bone.node_index) };
                DirectX::XMStoreFloat4x4(&data.bone_transforms[bone_index],
                    DirectX::XMLoadFloat4x4(&bone.offset_transform) *
                    DirectX::XMLoadFloat4x4(&bone_node.global_transform) *
                    DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mesh.default_global_transform))
                );

            }
        }
        else
        {
            XMStoreFloat4x4(&data.world, XMLoadFloat4x4(&mesh.default_global_transform) * DirectX::XMLoadFloat4x4(&defaultTransform) * XMLoadFloat4x4(&world));
            for (size_t bone_index = 0; bone_index < MAX_BONES; ++bone_index)
            {
                data.bone_transforms[bone_index] = { 1, 0, 0, 0,
                                                     0, 1, 0, 0,
                                                     0, 0, 1, 0,
                                                     0, 0, 0, 1 };

            }
        }
        for (const Subset& subset : mesh.subsets)
        {
            auto it = materialList.find(subset.material_unique_id);
            std::shared_ptr<Material> material = materialList.begin()->second;
            if (it != materialList.end())
                material = it->second;
            immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
            immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
            immediate_context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

            immediate_context->PSSetShaderResources(0, 1, material->getTextureShaderResource(0).GetAddressOf());

            immediate_context->DrawIndexedInstanced(subset.index_count, numIndex, subset.start_index_location, 0, 0);
        }
    }
}

SkinnedMesh::~SkinnedMesh()
{

}

void SkinnedMesh::SetDefaultTransform(DirectX::XMFLOAT4X4& tfDefault)
{
    defaultTransform = tfDefault;
    UpdateVertexMaxMinInfor();
}

void SkinnedMesh::UpdateVertexMaxMinInfor()
{
    for (auto& mesh : meshes)
    {
        VertexMinMaxInfor[0].x = std::min<float>(VertexMinMaxInfor[0].x, mesh.bounding_box[0].x);
        VertexMinMaxInfor[0].y = std::min<float>(VertexMinMaxInfor[0].y, mesh.bounding_box[0].y);
        VertexMinMaxInfor[0].z = std::min<float>(VertexMinMaxInfor[0].z, mesh.bounding_box[0].z);
        VertexMinMaxInfor[1].x = std::max<float>(VertexMinMaxInfor[1].x, mesh.bounding_box[1].x);
        VertexMinMaxInfor[1].y = std::max<float>(VertexMinMaxInfor[1].y, mesh.bounding_box[1].y);
        VertexMinMaxInfor[1].z = std::max<float>(VertexMinMaxInfor[1].z, mesh.bounding_box[1].z);
    }
    DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&VertexMinMaxInfor[0]);
    DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&defaultTransform);
    DirectX::XMStoreFloat3(&VertexMinMaxInfor[0], DirectX::XMVector3TransformCoord(V, M));
    V = DirectX::XMLoadFloat3(&VertexMinMaxInfor[1]);
    DirectX::XMStoreFloat3(&VertexMinMaxInfor[1], DirectX::XMVector3TransformCoord(V, M));
}

void SkinnedMesh::CreateNewCereal()
{
    std::filesystem::path cerealFileName(fileLocal);
    cerealFileName.replace_extension("mes");
    std::ofstream ofs(cerealFileName.c_str(), std::ios::binary);
    cereal::BinaryOutputArchive serialization(ofs);
    serialization(meshes, fileLocal, defaultTransform);
}

bool SkinnedMesh::IsCanLoadCereal(std::string local)
{
    fileLocal = local;
    std::filesystem::path cereal_filename(fileLocal);
    cereal_filename.replace_extension("mes");
    if (std::filesystem::exists(cereal_filename.c_str()))
    {
        std::ifstream ifs(cereal_filename.c_str(), std::ios::binary);
        cereal::BinaryInputArchive deserialization(ifs);
        deserialization(meshes, fileLocal, defaultTransform);
        return true;
    }
  
    return false;
}
