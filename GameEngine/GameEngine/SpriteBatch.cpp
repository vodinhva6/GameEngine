#include "SpriteBatch.h"
#include "GameEngine.h"
#include "misc.h"
#include <sstream>
#include <WICTextureLoader.h>
SpriteBatch::SpriteBatch(ID3D11Device* device, const wchar_t* filename, size_t max_sprites)
    : max_vertices(max_sprites * 6)
{
  

    std::unique_ptr<vertex[]> vertices{ std::make_unique<vertex[]>(max_vertices) };


    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth =(UINT)(sizeof(vertex) * max_vertices);
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    //buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = vertices.get();
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;

    HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    
   // ID3D11Resource* resource{};
    GameEngine::get()->LoadTextureFromFile(filename, shader_resource_view, &texture2d_desc);
   // resource->Release();
    //TextureManager::get().release_all_textures();



    
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //GameEngine::get()->getShaderManager
    //
    //ShaderManager* shaderManager = graphicEngine->getShaderManager();
    //hr = shaderManager->createVSFromFile_cso(device, "./Data/Shaders/sprite_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    //hr = shaderManager->createPSFromFile_cso(device, "./Data/Shaders/sprite_ps.cso", pixel_shader.GetAddressOf());
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


}
inline void rotate(float& x, float& y, float cx, float cy, float angle)
{
    x -= cx;
    y -= cy;

    float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
    float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
    float tx{ x }, ty{ y };
    x = cos * tx + -sin * ty;
    y = sin * tx + cos * ty;

    x += cx;
    y += cy;
}
void SpriteBatch::render(ID3D11DeviceContext* immediate_context,
    float dx, float dy, float dw, float dh,
    float r, float g, float b, float a,
    float angle,
    float sx, float sy, float sw, float sh
)
{
    D3D11_VIEWPORT viewport;
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);


    float x0{ dx };
    float y0{ dy };
    // right-top
    float x1{ dx + dw };
    float y1{ dy };
    // left-bottom
    float x2{ dx };
    float y2{ dy + dh };
    // right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };
    //auto rotate = [](float& x, float& y, float cx, float cy, float angle)
    //{
    //    x -= cx;
    //    y -= cy;
    //
    //    float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
    //    float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
    //    float tx{ x }, ty{ y };
    //    x = cos * tx + -sin * ty;
    //    y = sin * tx + cos * ty;
    //
    //    x += cx;
    //    y += cy;
    //};

    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;
    rotate(x0, y0, cx, cy, angle);
    rotate(x1, y1, cx, cy, angle);
    rotate(x2, y2, cx, cy, angle);
    rotate(x3, y3, cx, cy, angle);

    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;

    float u0{ sx / texture2d_desc.Width };
    float v0{ sy / texture2d_desc.Height };
    float u1{ (sx + sw) / texture2d_desc.Width };
    float v1{ (sy + sh) / texture2d_desc.Height };


    vertices.push_back({ { x0, y0 , 0 }, { r, g, b, a }, { u0, v0 } });
    vertices.push_back({ { x1, y1 , 0 }, { r, g, b, a }, { u1, v0 } });
    vertices.push_back({ { x2, y2 , 0 }, { r, g, b, a }, { u0, v1 } });
    vertices.push_back({ { x2, y2 , 0 }, { r, g, b, a }, { u0, v1 } });
    vertices.push_back({ { x1, y1 , 0 }, { r, g, b, a }, { u1, v0 } });
    vertices.push_back({ { x3, y3 , 0 }, { r, g, b, a }, { u1, v1 } });

    //HRESULT hr{ S_OK };
    //D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    //hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    //vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    //if (vertices != nullptr)
    //{

    //    vertices[0].position = { x0, y0 , 0 };
    //    vertices[1].position = { x1, y1 , 0 };
    //    vertices[2].position = { x2, y2 , 0 };
    //    vertices[3].position = { x3, y3 , 0 };
    //    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r, g, b, a };
    //    vertices[0].texcoord = { 0, 0 };
    //    vertices[1].texcoord = { (sx + sw) / texture2d_desc.Width, 0 };
    //    vertices[2].texcoord = { 0, (sy + sh) / texture2d_desc.Height };
    //    vertices[3].texcoord = { (sx + sw) / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
    //}

    //immediate_context->Unmap(vertex_buffer.Get(), 0);


    //UINT stride{ sizeof(vertex) };
    //UINT offset{ 0 };
    //immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    //immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    //immediate_context->IASetInputLayout(input_layout.Get());
    //immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
    //immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    //immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
    //immediate_context->Draw(4, 0);




}
void SpriteBatch::render(ID3D11DeviceContext* immediate_context,
    float dx, float dy, float dw, float dh,
    float r, float g, float b, float a,
    float angle
)
{
    D3D11_VIEWPORT viewport;
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);


    float x0{ dx };
    float y0{ dy };
    // right-top
    float x1{ dx + dw };
    float y1{ dy };
    // left-bottom
    float x2{ dx };
    float y2{ dy + dh };
    // right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };
    //auto rotate = [](float& x, float& y, float cx, float cy, float angle)
    //{
    //    x -= cx;
    //    y -= cy;
    //
    //    float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
    //    float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
    //    float tx{ x }, ty{ y };
    //    x = cos * tx + -sin * ty;
    //    y = sin * tx + cos * ty;
    //
    //    x += cx;
    //    y += cy;
    //};

    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;
    rotate(x0, y0, cx, cy, angle);
    rotate(x1, y1, cx, cy, angle);
    rotate(x2, y2, cx, cy, angle);
    rotate(x3, y3, cx, cy, angle);

    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;


    

    //HRESULT hr{ S_OK };
    //D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    //hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    //vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    //if (vertices != nullptr)
    //{

    //    vertices[0].position = { x0, y0 , 0 };
    //    vertices[1].position = { x1, y1 , 0 };
    //    vertices[2].position = { x2, y2 , 0 };
    //    vertices[3].position = { x3, y3 , 0 };
    //    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r, g, b, a };
    //    vertices[0].texcoord = { 0, 0 };
    //    vertices[1].texcoord = { 1, 0 };
    //    vertices[2].texcoord = { 0, 1 };
    //    vertices[3].texcoord = { 1, 1 };
    //}

    //immediate_context->Unmap(vertex_buffer.Get(), 0);


    //UINT stride{ sizeof(vertex) };
    //UINT offset{ 0 };
    //immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    //immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    //immediate_context->IASetInputLayout(input_layout.Get());
    //immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
    //immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    //immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
    //immediate_context->Draw(4, 0);




}
void SpriteBatch::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh)
{
    D3D11_VIEWPORT viewport;
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);


    float x0{ dx };
    float y0{ dy };
    // right-top
    float x1{ dx + dw };
    float y1{ dy };
    // left-bottom
    float x2{ dx };
    float y2{ dy + dh };
    // right-bottom
    float x3{ dx + dw };
    float y3{ dy + dh };
    //auto rotate = [](float& x, float& y, float cx, float cy, float angle)
    //{
    //    x -= cx;
    //    y -= cy;
    //
    //    float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
    //    float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
    //    float tx{ x }, ty{ y };
    //    x = cos * tx + -sin * ty;
    //    y = sin * tx + cos * ty;
    //
    //    x += cx;
    //    y += cy;
    //};

    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;
    rotate(x0, y0, cx, cy, 0);
    rotate(x1, y1, cx, cy, 0);
    rotate(x2, y2, cx, cy, 0);
    rotate(x3, y3, cx, cy, 0);

    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;




    //HRESULT hr{ S_OK };
    //D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    //hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    //_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    //vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    //if (vertices != nullptr)
    //{

    //    vertices[0].position = { x0, y0 , 0 };
    //    vertices[1].position = { x1, y1 , 0 };
    //    vertices[2].position = { x2, y2 , 0 };
    //    vertices[3].position = { x3, y3 , 0 };
    //    vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { 1, 1, 1, 1 };
    //    vertices[0].texcoord = { 0, 0 };
    //    vertices[1].texcoord = { 1, 0 };
    //    vertices[2].texcoord = { 0, 1 };
    //    vertices[3].texcoord = { 1, 1 };
    //}

    //immediate_context->Unmap(vertex_buffer.Get(), 0);


    //UINT stride{ sizeof(vertex) };
    //UINT offset{ 0 };
    //immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    //immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    //immediate_context->IASetInputLayout(input_layout.Get());
    //immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
    //immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    //immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
    //immediate_context->Draw(4, 0);

}

void SpriteBatch::begin(ID3D11DeviceContext* immediate_context)
{
    vertices.clear();
    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
    immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
}

void SpriteBatch::end(ID3D11DeviceContext* immediate_context)
{
    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    
    size_t vertex_count = vertices.size();
    _ASSERT_EXPR(max_vertices >= vertex_count, "Buffer overflow");
    vertex * data{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (data != nullptr)
    {
        const vertex * p = vertices.data();
        memcpy_s(data, max_vertices * sizeof(vertex), p, vertex_count * sizeof(vertex));
    }
    immediate_context->Unmap(vertex_buffer.Get(), 0);

    UINT stride{ sizeof(vertex) };
    UINT offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(input_layout.Get());
    
    immediate_context->Draw(static_cast<UINT>(vertex_count), 0);


}


SpriteBatch::~SpriteBatch()
{

}
