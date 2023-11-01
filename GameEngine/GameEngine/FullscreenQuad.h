#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <cstdint>
class FullscreenQuad
{
public:
    FullscreenQuad(ID3D11Device* device);
    virtual~FullscreenQuad() = default;

private:

public:

    void blit(ID3D11DeviceContext* immediate_context, ID3D11ShaderResourceView** shader_resource_view,
        uint32_t start_slot, uint32_t num_views);
    void blit2ShaderResourceView(ID3D11DeviceContext* immediate_context,
        ID3D11ShaderResourceView** shader_resource_view1, ID3D11ShaderResourceView** shader_resource_view2, 
        uint32_t start_slot, uint32_t num_views);
    void blit2ShaderResourceView2DepthStencilView(ID3D11DeviceContext* immediate_context, 
        ID3D11ShaderResourceView** shader_resource_view1, ID3D11ShaderResourceView** shader_resource_view2,
        ID3D11ShaderResourceView** depthStencil1, ID3D11ShaderResourceView** depthStencil2,
        uint32_t start_slot, uint32_t num_views);

    void BlitFromNumResourceView(ID3D11DeviceContext* immediateContext,
        ID3D11ShaderResourceView** shaderResourceView, uint32_t startSlot, uint32_t num);
};

