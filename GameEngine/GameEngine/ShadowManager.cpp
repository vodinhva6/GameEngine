#include "CameraManager.h"
#include "GameEngine.h"
#include "FrameBufferManager.h"
#include "LightManager.h"
#include <limits>
#undef max
#undef min
#include <algorithm>
#include <array>
#ifdef USE_IMGUI
#include "UsesImGui_h.h"
#endif // USE_IMGUI



std::array<VECTOR4, 8> _make_frustum_corners_world_space(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	const DirectX::XMMATRIX VP_ = DirectX::XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&projection));

	std::array<VECTOR4, 8> frustum_corners;
	size_t index = 0;
	for (size_t x = 0; x < 2; ++x)
	{
		for (size_t y = 0; y < 2; ++y)
		{
			for (size_t z = 0; z < 2; ++z)
			{
				VECTOR4 pt = { 2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f };
				DirectX::XMStoreFloat4(&pt, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&pt), VP_));
				frustum_corners.at(index++) = pt;
			}
		}
	}
	return frustum_corners;
}

ShadowManager::ShadowManager()
{
	directShadowMapInfor.cascadeCount = 4;
	directShadowMapInfor.splitCchemeWeight = 0.7f;

	directShadowMapInfor.shadowDepthBias = 0.00002f;
	directShadowMapInfor.shadowColor = 0.25f;
	directShadowMapInfor.shadowFilterRadius = 4.000f;
	directShadowMapInfor.shadowSampleCount = 16;
}

void ShadowManager::Init()
{
	FrameBufferManager* frameBufferManager;
	frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());

	directShadowMapInfor.cascadeCount = frameBufferManager->getDirectionLightShadowCascadeCount();


	for (size_t i = 0; i < 10; i++)
	{
		pointLightsShadowMapInfor[i].slideCount = frameBufferManager->getPointLightShadowMapSlide();
	}
	
	

}

void ShadowManager::Update()
{
	//UpdateProjection();
	CreateDirectionLightShadowMap();
	CreatePointLightsShadowMap();

#ifdef USE_IMGUI
	ImGui::Indent();
	ImGui::DragFloat("shadow_depth_bias", &directShadowMapInfor.shadowDepthBias, 0.000001f, 0.0f, 0.01f, "%.8f");
	ImGui::SliderFloat("shadow_color", &directShadowMapInfor.shadowColor, 0.0f, 1.0f);
	ImGui::SliderFloat("shadow_filter_radius", &directShadowMapInfor.shadowFilterRadius, 0.0f, 64.0f);
	ImGui::SliderInt("shadow_sample_count", reinterpret_cast<int*>(&directShadowMapInfor.shadowSampleCount), 0, 64);
	ImGui::Unindent();
#endif // !USES_IMGUI

}

ShadowManager::~ShadowManager()
{
}

void ShadowManager::UpdateProjection()
{
	D3D11_VIEWPORT viewport;
	UINT num_viewports{ 1 };
	FrameBufferManager* frameBufferManager = GetFrom<FrameBufferManager>(GameEngine::get()->getFrameBufferManager());
	viewport = frameBufferManager->getViewPort(FrameBufferName::DIRECTIONLIGHTSHADOWFRAMEBUFFER);
	float aspect_ratio = viewport.Width / viewport.Height;
}

void ShadowManager::CreateDirectionLightShadowMap()
{
	LightManager* lightManager = GetFrom<LightManager>(GameEngine::get()->getLightManager());
	CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());
	VECTOR4 lightDirection = lightManager->getLightDirection();

	// near/far value from perspective projection matrix

	float zoomNear = cameraManager->getGameCameraNear();
	float zoomFar = std::min(cameraManager->getGameCameraFar(), 700.0f);


	// calculates split plane distances in view space
	directShadowMapInfor.distances.resize(static_cast<size_t>(directShadowMapInfor.cascadeCount) + 1);
	for (size_t cascadeIndex = 0; cascadeIndex < directShadowMapInfor.cascadeCount; ++cascadeIndex)
	{
		float idc = cascadeIndex / static_cast<float>(directShadowMapInfor.cascadeCount);
		float logarithmic_split_scheme = zoomNear * pow(zoomFar / zoomNear, idc);
		float uniform_split_scheme = zoomNear + (zoomFar - zoomNear) * idc;
		directShadowMapInfor.distances.at(cascadeIndex) = logarithmic_split_scheme * directShadowMapInfor.splitCchemeWeight + uniform_split_scheme * (1 - directShadowMapInfor.splitCchemeWeight);
	}
	// make sure border values are accurate
	directShadowMapInfor.distances.at(0) = zoomNear;
	directShadowMapInfor.distances.at(directShadowMapInfor.cascadeCount) = zoomFar;
	
	const bool fit_to_cascade = true; // fit to scene : fit to cascade
	directShadowMapInfor.viewProjections.resize(directShadowMapInfor.cascadeCount);
	for (size_t cascadeIndex = 0; cascadeIndex < directShadowMapInfor.cascadeCount; ++cascadeIndex)
	{
		float _zn = fit_to_cascade ? directShadowMapInfor.distances.at(cascadeIndex) : zoomNear;
		float _zf = directShadowMapInfor.distances.at(cascadeIndex + 1);
	
		DirectX::XMFLOAT4X4 cascadedProjection;
		DirectX::XMFLOAT4X4 cameraView;
		DirectX::XMStoreFloat4x4(&cameraView, cameraManager->GetGameCameraView3D());
		DirectX::XMStoreFloat4x4(&cascadedProjection, cameraManager->GetGameCameraProjection3D());
		cascadedProjection._33 = _zf / (_zf - _zn);
		cascadedProjection._43 = -_zn * _zf / (_zf - _zn);
	
		std::array<VECTOR4, 8> corners = _make_frustum_corners_world_space(cameraView, cascadedProjection);
	
		DirectX::XMFLOAT4 center = { 0, 0, 0, 1 };
		for (decltype(corners)::const_reference v : corners)
		{
			center.x += v.x;
			center.y += v.y;
			center.z += v.z;
		}
		center.x /= corners.size();
		center.y /= corners.size();
		center.z /= corners.size();
	
		DirectX::XMMATRIX V;
		V = DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(center.x - lightDirection.x, center.y - lightDirection.y, center.z - lightDirection.z, 1.0f),
			DirectX::XMVectorSet(center.x, center.y, center.z, 1.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	
		float min_x = std::numeric_limits<float>::max();
		float max_x = std::numeric_limits<float>::lowest();
		float min_y = std::numeric_limits<float>::max();
		float max_y = std::numeric_limits<float>::lowest();
		float min_z = std::numeric_limits<float>::max();
		float max_z = std::numeric_limits<float>::lowest();
		for (decltype(corners)::const_reference v : corners)
		{
			DirectX::XMFLOAT4 _v;
			DirectX::XMStoreFloat4(&_v, XMVector3TransformCoord(XMLoadFloat4(&v), V));
			min_x = std::min(min_x, _v.x);
			max_x = std::max(max_x, _v.x);
			min_y = std::min(min_y, _v.y);
			max_y = std::max(max_y, _v.y);
			min_z = std::min(min_z, _v.z);
			max_z = std::max(max_z, _v.z);
		}

#if 1
		// Before creating the actual projection matrix we are going to increase the size of the space covered by the nearand far plane of the light frustum.
		// We do this by "pulling back" the near plane, and "pushing away" the far plane.In the code we achieve this by dividing or multiplying by zMult.
		// This is because we want to include geometry which is behind or in front of our frustum in camera space. Think about it : not only geometry which 
		// is in the frustum can cast shadows on a surface in the frustum!
		constexpr float z_mult = 50.0f;
		if (min_z < 0)
		{
			min_z *= z_mult;
		}
		else
		{
			min_z /= z_mult;
		}
		if (max_z < 0)
		{
			max_z /= z_mult;
		}
		else
		{
			max_z *= z_mult;
		}
#endif

		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicOffCenterLH(min_x, max_x, min_y, max_y, min_z, max_z);
		DirectX::XMStoreFloat4x4(&directShadowMapInfor.viewProjections.at(cascadeIndex), V * P);
	}
}

void ShadowManager::CreatePointLightsShadowMap()
{
	CameraManager* cameraManager = GetFrom<CameraManager>(GameEngine::get()->getCameraManager());
	LightManager* lightManager = GetFrom<LightManager>(GameEngine::get()->getLightManager());
	size_t sizeList = lightManager->getSizePointLightList();


	float zoomNear = cameraManager->getGameCameraNear();
	float zoomFar = std::min(cameraManager->getGameCameraFar(), 200.0f);


	

	VECTOR3 focusDir[6] =
	{
		{1,0,0},
		{-1,0,0},
		{0,1,0},
		{0,-1,0},
		{0,0,1},
		{0,0,-1}
	};

	VECTOR3 upEye[6] =
	{
		{0,1,0},
		{0,1,0},
		{0,0,-1},
		{0,0,1},
		{0,1,0},
		{0,1,0},
	};
	static float angle = 90, nearr = 0.1f, farr = 20;

	ImGui::Begin("Tesst");
	ImGui::SliderFloat("Angle",&angle, 1,180);
	ImGui::SliderFloat("near", &nearr, 0.1f, 10);
	ImGui::SliderFloat("far", &farr, 20, 1000);
	ImGui::End();
	for (size_t slot = 0; slot < sizeList; slot++)
	{
		if (!lightManager->getPointLightAt(slot)->getShadowCast()) continue;
		pointLightsShadowMapInfor[slot].distances = 200;
		pointLightsShadowMapInfor[slot].viewProjections.resize(pointLightsShadowMapInfor[slot].slideCount);
		for (UINT i = 0; i < pointLightsShadowMapInfor[slot].slideCount; i++)
		{
			VECTOR3 eye = lightManager->getPointLightAt(slot)->getPosition();
			VECTOR3 up = upEye[i];
			VECTOR3 focus = eye + focusDir[i];
			float range = lightManager->getPointLightAt(slot)->getRange();

			DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&focus), DirectX::XMLoadFloat3(&up));
			DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(angle), 1, nearr, range + 50);
			
			DirectX::XMStoreFloat4x4(&pointLightsShadowMapInfor[slot].viewProjections[i], V * P);
		};
	}
	



}
