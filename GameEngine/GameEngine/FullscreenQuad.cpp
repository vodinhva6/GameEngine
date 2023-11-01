#include "FullscreenQuad.h"
#include "GameEngine.h"
#include "Misc.h"


FullscreenQuad::FullscreenQuad(ID3D11Device* device)
{

}

void FullscreenQuad::blit(ID3D11DeviceContext* immediate_context, ID3D11ShaderResourceView** shader_resource_view, uint32_t start_slot, uint32_t num_views)
{
    immediate_context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    immediate_context->IASetInputLayout(nullptr);

    immediate_context->PSSetShaderResources(start_slot, num_views, shader_resource_view);

    immediate_context->Draw(4, 0);

    std::vector<ID3D11ShaderResourceView*> dummyV;
    dummyV.resize(num_views);
    ID3D11ShaderResourceView* dummy = nullptr;
    immediate_context->PSSetShaderResources(start_slot, num_views, dummyV.data());
}

void FullscreenQuad::blit2ShaderResourceView(ID3D11DeviceContext* immediate_context,
    ID3D11ShaderResourceView** shader_resource_view1, ID3D11ShaderResourceView** shader_resource_view2,
    uint32_t start_slot, uint32_t num_views)
{
    immediate_context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    immediate_context->IASetInputLayout(nullptr);

    immediate_context->PSSetShaderResources(start_slot, num_views, shader_resource_view1);
    immediate_context->PSSetShaderResources(start_slot+1, num_views, shader_resource_view2);
    immediate_context->Draw(4, 0);

    ID3D11ShaderResourceView* dummy = nullptr;
    immediate_context->PSSetShaderResources(start_slot, num_views, &dummy);
    immediate_context->PSSetShaderResources(start_slot+1, num_views, &dummy);
}

void FullscreenQuad::blit2ShaderResourceView2DepthStencilView(ID3D11DeviceContext* immediate_context, ID3D11ShaderResourceView** shader_resource_view1, ID3D11ShaderResourceView** shader_resource_view2, ID3D11ShaderResourceView** depthStencil1, ID3D11ShaderResourceView** depthStencil2, uint32_t start_slot, uint32_t num_views)
{
    immediate_context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    immediate_context->IASetInputLayout(nullptr);

    immediate_context->PSSetShaderResources(start_slot, num_views, shader_resource_view1);
    immediate_context->PSSetShaderResources(start_slot + 1, num_views, depthStencil1);
    immediate_context->PSSetShaderResources(start_slot + 2, num_views, shader_resource_view2);
    immediate_context->PSSetShaderResources(start_slot + 3, num_views, depthStencil2);

    immediate_context->Draw(4, 0);

    ID3D11ShaderResourceView* dummy = nullptr;
    immediate_context->PSSetShaderResources(start_slot, num_views, &dummy);
    immediate_context->PSSetShaderResources(start_slot + 1, num_views, &dummy);
    immediate_context->PSSetShaderResources(start_slot + 2, num_views, &dummy);
    immediate_context->PSSetShaderResources(start_slot + 3, num_views, &dummy);
}

void FullscreenQuad::BlitFromNumResourceView(ID3D11DeviceContext* immediateContext, ID3D11ShaderResourceView** shaderResourceView, uint32_t startSlot, uint32_t num)
{
    immediateContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    immediateContext->IASetInputLayout(nullptr);

    immediateContext->PSSetShaderResources(startSlot, num, shaderResourceView);

    for (uint32_t i = 0; i < num; i++)
    {
      
    }
    

    immediateContext->Draw(4, 0);

    for (uint32_t i = 0; i < num; i++)
    {
        ID3D11ShaderResourceView* dummy = nullptr;
        immediateContext->PSSetShaderResources(i, 1, &dummy);
    }
  

}
