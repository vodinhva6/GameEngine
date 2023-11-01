#include "ConstantBuffer.h"
#include "Misc.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "GameEngine.h"
void SceneBuffer::CreateBuffer(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.ByteWidth = sizeof(SceneConstant);
	hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	PSSlotNum = 1;
	VSSlotNum = 1;
	sceneConstant.motisonBlurSample = 1;
}

void SceneBuffer::UpdateBuffer(ID3D11DeviceContext* dcContext)
{
#ifdef USE_IMGUI
	ImGui::Begin("SceneBuffer");
	ImGui::DragInt("MotionBlurSampleNum", &sceneConstant.motisonBlurSample,1, 0, INT_MAX);
	ImGui::End();
#endif // USE_IMGUI

	CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());

	sceneConstant.gameCameraPosition = cameraManager->getGameCameraPosition();
	sceneConstant.debugCameraPosition = cameraManager->getDebugCameraPosition();
	XMStoreFloat4x4(&sceneConstant.viewProjectionNow, cameraManager->GetSceneView3DNow());
	XMStoreFloat4x4(&sceneConstant.debugViewProjection, cameraManager->GetDebugSceneView3D());
	XMStoreFloat4x4(&sceneConstant.debugViewProjectionInvers, DirectX::XMMatrixInverse(nullptr,cameraManager->GetDebugSceneView3D()));
	XMStoreFloat4x4(&sceneConstant.viewProjectionNowInvers, DirectX::XMMatrixInverse(nullptr,cameraManager->GetSceneView3DNow()));
	XMStoreFloat4x4(&sceneConstant.viewProjectionOld, cameraManager->GetSceneView3DOld());
	XMStoreFloat4x4(&sceneConstant.projectionNowInvers, DirectX::XMMatrixInverse(nullptr,cameraManager->GetGameCameraProjection3D()));
	dcContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &sceneConstant, 0, 0);
}


void ColorFilterBuffer::CreateBuffer(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.ByteWidth = sizeof(ColorFilter);
	hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	PSSlotNum = 2;
	VSSlotNum = 2;
	colorFilerConstant.brightness = 1;
	colorFilerConstant.gamma = 2.2f;
	colorFilerConstant.hueShift = 1;
	colorFilerConstant.saturation = 1;
}

void ColorFilterBuffer::UpdateBuffer(ID3D11DeviceContext* dcContext)
{
#ifdef USE_IMGUI
	ImGui::Begin("ColorFilter");
	ImGui::DragFloat("HueShift", &colorFilerConstant.hueShift, 0.003f,-10.0f, 10.0f);
	ImGui::DragFloat("Saturation", &colorFilerConstant.saturation, 0.003f, -10.0f, 10.0f);
	ImGui::DragFloat("Brightness", &colorFilerConstant.brightness, 0.003f, -10.0f, 10.0f);
	ImGui::DragFloat("Gamma", &colorFilerConstant.gamma, 0.003f, 0.1f, 3.0f);
	ImGui::End();
#endif // USE_IMGUI

	dcContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &colorFilerConstant, 0, 0);
}


void FogBuffer::CreateBuffer(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.ByteWidth = sizeof(FogConstant);
	hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	PSSlotNum = 6;
	VSSlotNum = 6;
}

void FogBuffer::UpdateBuffer(ID3D11DeviceContext* dcContext)
{

	//FogConstant fogConstant;
	//fogConstant.fogRange = fogRange_;
	//fogConstant.fogColor = fogColor_;
	dcContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &fogConstant, 0, 0);

}

