#include "Mesh.h"
#include <Misc.h>
Meshes::Meshes()
{
    DirectX::XMStoreFloat4x4(&defaultTransform, DirectX::XMMatrixIdentity());

    VertexMinMaxInfor[0] = { D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX };
    VertexMinMaxInfor[1] = { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX };


}

void Meshes::CreateComObjects(ID3D11Device* device)
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(Constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void Meshes::SetDefaultTransform(DirectX::XMFLOAT4X4& tfDefault)
{
    defaultTransform = tfDefault;
    UpdateVertexMaxMinInfor();
}
