#include "Mesh.h"

Mesh::Mesh()
{
    DirectX::XMStoreFloat4x4(&defaultTransform, DirectX::XMMatrixIdentity());

    VertexMinMaxInfor[0] = { D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX };
    VertexMinMaxInfor[1] = { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX };


}

void Mesh::SetDefaultTransform(DirectX::XMFLOAT4X4& tfDefault)
{
    defaultTransform = tfDefault;
    UpdateVertexMaxMinInfor();
}
