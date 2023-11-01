#include "ConstantBloom.h"
#include "Log.h"
void BloomBuffer::CreateBuffer(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.ByteWidth = sizeof(Bloom);
	hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
	SUCCEEDEDRESULT(hr);
	PSSlotNum = 8; 
	bloomConstant.bloomExtractionThreshold = 1;
	bloomConstant.bloomIntensity = 1;
}

void BloomBuffer::UpdateBuffer(ID3D11DeviceContext* dcContext)
{
#ifdef USE_IMGUI
	ImGui::Begin("BloomConstant");
	ImGui::DragFloat("BloomExtractionThreshold", &bloomConstant.bloomExtractionThreshold, 0.003f, 0, 10.0f);
	ImGui::DragFloat("BloomIntensity", &bloomConstant.bloomIntensity, 0.003f, -10.0f, 10.0f);
	ImGui::End();
#endif // USE_IMGUI

	dcContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &bloomConstant, 0, 0);
}
