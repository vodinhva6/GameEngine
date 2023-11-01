#include "Sprite3D.h"
#include "GameEngine.h"
#include "Misc.h"
Sprite3D::Sprite3D(ID3D11Device* device, std::filesystem::path local)
{
    
    Vertex vertex[] =
    {
    { { -1.0, +1.0, 0 }, { 0, 0 } },
    { { +1.0, +1.0, 0 }, { 1, 0 } },
    { { +1.0, -1.0, 0 }, { 1, 1 } },
    { { -1.0, -1.0, 0 }, { 0, 1 } },
    };
    int sizeVertex = sizeof(vertex) / sizeof(Vertex);
    for (int i = 0; i < sizeVertex; i++)
    {
        vertices.push_back(vertex[i]);
    }

    uint32_t index[] =
    {
        0,1,2,
        2,3,0
    };
    uint32_t sizeindices = sizeof(index) / sizeof(uint32_t);
    for (uint32_t i = 0; i < sizeindices; i++)
    {
        indices.push_back(index[i]);
    }
    

    HRESULT hr{ S_OK };
    D3D11_BUFFER_DESC buffer_desc{};
    D3D11_SUBRESOURCE_DATA subresource_data{};
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * sizeVertex);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    subresource_data.pSysMem = &vertices.at(0);
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * sizeindices);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresource_data.pSysMem = &indices.at(0);
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, indexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



    {
        D3D11_BUFFER_DESC buffer_desc{};
        buffer_desc.ByteWidth = sizeof(Constants);
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    
    GameEngine::get()->LoadTextureFromFile(local, shaderResourceView, &texture2dDesc);
    
   
}

void Sprite3D::updateAnimation()
{

}

void Sprite3D::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
    uint32_t stride{ sizeof(Vertex) };
    uint32_t offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //immediate_context->IASetInputLayout(input_layout.Get());
    //immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    //immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);


    Constants data{ world, material_color };
    immediate_context->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
    immediate_context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
    immediate_context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
    immediate_context->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    indexBuffer.Get()->GetDesc(&buffer_desc);
    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
}
