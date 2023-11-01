#include "Sprite.h"
#include "GameEngine.h"
#include "Misc.h"
#include <WICTextureLoader.h>
#include <sstream> 

Sprite::Sprite(ID3D11Device* device, const wchar_t* filename, ShaderData shaderData)
{
    createVertexData(device, filename);
    createShaderData(device, shaderData);
}
Sprite::Sprite(ID3D11Device* device, const wchar_t* filename)
{
    ShaderData shaderData = { "./Data/Shaders/sprite_vs.cso" ,"./Data/Shaders/sprite_ps.cso" };
    createVertexData(device, filename);
    createShaderData(device, shaderData);
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
void Sprite::textout(ID3D11DeviceContext* immediate_context, std::string s,
    float x, float y, float w, float h, float r, float g, float b, float a)
{
    float sw = static_cast<float>(texture2d_desc.Width / 16);
    float sh = static_cast<float>(texture2d_desc.Height / 16);
    float carriage = 0;
    for (const char c : s)
    {
        render(immediate_context, x + carriage, y, w, h, r, g, b, a, 0,
            sw * (c & 0x0F), sh * (c >> 4), sw, sh);
        carriage += w;
    }
}
void Sprite::render(ID3D11DeviceContext* immediate_context,
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

    


    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0 , 0 };
        vertices[1].position = { x1, y1 , 0 };
        vertices[2].position = { x2, y2 , 0 };
        vertices[3].position = { x3, y3 , 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r, g, b, a };
        vertices[0].texcoord = { sx / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[1].texcoord = { (sx + sw) / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[2].texcoord = { sx / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
        vertices[3].texcoord = { (sx + sw) / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
    }
    
    immediate_context->Unmap(vertex_buffer.Get(), 0);


    draw(immediate_context);
    
   


}

void Sprite::render(ID3D11DeviceContext* immediate_context,
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




    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {

        vertices[0].position = { x0, y0 , 0 };
        vertices[1].position = { x1, y1 , 0 };
        vertices[2].position = { x2, y2 , 0 };
        vertices[3].position = { x3, y3 , 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r, g, b, a };
        vertices[0].texcoord = { 0, 0 };
        vertices[1].texcoord = { 1, 0 };
        vertices[2].texcoord = { 0, 1 };
        vertices[3].texcoord = { 1, 1 };
    }

    immediate_context->Unmap(vertex_buffer.Get(), 0);

    draw(immediate_context);




}

void Sprite::render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh)
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




    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {

        vertices[0].position = { x0, y0 , 0 };
        vertices[1].position = { x1, y1 , 0 };
        vertices[2].position = { x2, y2 , 0 };
        vertices[3].position = { x3, y3 , 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { 1, 1, 1, 1 };
        vertices[0].texcoord = { 0, 0 };
        vertices[1].texcoord = { 1, 0 };
        vertices[2].texcoord = { 0, 1 };
        vertices[3].texcoord = { 1, 1 };
    }

    immediate_context->Unmap(vertex_buffer.Get(), 0);


    draw(immediate_context);
}

Sprite::~Sprite()
{
    

}

void Sprite::render(ID3D11DeviceContext* immediate_context, 
    float dx, float dy, 
    float dw, float dh,
    float cx, float cy,
    float r, float g, float b, float a, 
    float angle, 
    float sx, float sy, 
    float sw, float sh)
{
    D3D11_VIEWPORT viewport;
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);

    float x0{ dx - cx };
    float y0{ dy - cy };
    // right-top
    float x1{ dx + dw - cx  };
    float y1{ dy - cy  };
    // left-bottom
    float x2{ dx - cx  };
    float y2{ dy + dh - cy  };
    // right-bottom
    float x3{ dx + dw - cx  };
    float y3{ dy + dh - cy  };

    float cx0 = x0 + cx;
    float cy0 = y0 + cy;
    rotate(x0, y0, cx0, cy0, angle);
    rotate(x1, y1, cx0, cy0, angle);
    rotate(x2, y2, cx0, cy0, angle);
    rotate(x3, y3, cx0, cy0, angle);

    //outRange = { x0,y0 };
    //size = { x3 - x0,y3 - y0 };


    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;




    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0 , 0 };
        vertices[1].position = { x1, y1 , 0 };
        vertices[2].position = { x2, y2 , 0 };
        vertices[3].position = { x3, y3 , 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r, g, b, a };
        vertices[0].texcoord = { sx / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[1].texcoord = { (sx + sw) / texture2d_desc.Width, sy / texture2d_desc.Height };
        vertices[2].texcoord = { sx / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
        vertices[3].texcoord = { (sx + sw) / texture2d_desc.Width, (sy + sh) / texture2d_desc.Height };
    }

    immediate_context->Unmap(vertex_buffer.Get(), 0);


    draw(immediate_context);

}

void Sprite::render(ID3D11DeviceContext* immediate_context, 
    float dx, float dy, 
    float dw, float dh,
    float cx, float cy,
    float r, float g, float b, float a, 
    float angle)
{
    D3D11_VIEWPORT viewport;
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);

    float x0{ dx - cx };
    float y0{ dy - cy };
    // right-top
    float x1{ dx + dw - cx };
    float y1{ dy - cy };
    // left-bottom
    float x2{ dx - cx };
    float y2{ dy + dh - cy };
    // right-bottom
    float x3{ dx + dw - cx };
    float y3{ dy + dh - cy };

    float cx0 = x0 + cx;
    float cy0 = y0 + cy;
    rotate(x0, y0, cx0, cy0, angle);
    rotate(x1, y1, cx0, cy0, angle);
    rotate(x2, y2, cx0, cy0, angle);
    rotate(x3, y3, cx0, cy0, angle);


    //outRange = { x0,y0 };
    //size = { x3 - x0,y3 - y0 };




    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;




    HRESULT hr{ S_OK };
    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
    hr = immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0 , 0 };
        vertices[1].position = { x1, y1 , 0 };
        vertices[2].position = { x2, y2 , 0 };
        vertices[3].position = { x3, y3 , 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r, g, b, a };
        vertices[0].texcoord = { 0, 0 };
        vertices[1].texcoord = { 1, 0 };
        vertices[2].texcoord = { 0, 1 };
        vertices[3].texcoord = { 1, 1 };
    }

    immediate_context->Unmap(vertex_buffer.Get(), 0);

    draw(immediate_context);
    
}

void Sprite::draw(ID3D11DeviceContext* immediate_context)
{
    UINT stride{ sizeof(vertex) };
    UINT offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    //immediate_context->IASetInputLayout(input_layout.Get());
    //immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    //immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
    immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
    immediate_context->Draw(4, 0);
}

void Sprite::createShaderData(ID3D11Device* device, ShaderData shaderData)
{

   /* D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    HRESULT hr = ShaderManager::get()->create_vs_from_cso(device, shaderData.vsfile, vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    hr = ShaderManager::get()->create_ps_from_cso(device, shaderData.psfile, pixel_shader.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));*/

}

void Sprite::createVertexData(ID3D11Device* device, const wchar_t* filename)
{
    vertex vertices[]
    {
     { { -1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } },
     { { +1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 1, 0 } },
     { { -1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
     { { +1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
    };



    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(vertices);
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
    //buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = vertices;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;


    HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    GameEngine::get()->LoadTextureFromFile(filename, shader_resource_view, &texture2d_desc);
    
}
