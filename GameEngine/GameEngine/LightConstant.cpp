#include "LightConstant.h"
#include "GameEngine.h"
#include "Misc.h"
#include "LightManager.h"

void LightBuffer::CreateBuffer(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.ByteWidth = (sizeof(LightConstants) + 0x0f) & ~0x0f;;
	hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	PSSlotNum = 4;
	VSSlotNum = 4;



}

void LightBuffer::UpdateBuffer(ID3D11DeviceContext* dcContext)
{
	
	UpdateConstantData();

	D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mapped_buffer;

	HRESULT hr = dcContext->Map(constantBuffer.Get(), 0, map, 0, &mapped_buffer);
	assert(SUCCEEDED(hr) && hr_trace(hr));
	memcpy_s(mapped_buffer.pData, sizeof(LightConstants), &lightConstant, sizeof(LightConstants));
	dcContext->Unmap(constantBuffer.Get(), 0);
}

void LightBuffer::UpdateConstantData()
{
	UpdateDirectionLightData();
	UpdatePointLightData();
	
}

void LightBuffer::UpdateDirectionLightData()
{
	LightManager* lightManager = GetFrom<LightManager>(GameEngine::get()->getLightManager());
	ShadowManager* shadowManager = GetFrom<LightManager>(GameEngine::get()->getLightManager())->getShadowManager();

	DirectionLightData& directionLightData = lightConstant.directionLightData;

	directionLightData.lightDirection = lightManager->getLightDirection();
	directionLightData.lightColor = lightManager->getLightColor();

	UpdateDirectionLightShadowMapData();
}

void LightBuffer::UpdateDirectionLightShadowMapData()
{
	ShadowManager* shadowManager = GetFrom<LightManager>(GameEngine::get()->getLightManager())->getShadowManager();

	DirectionLightData::DirectionLightShadowMapData& directionLightShadowMapData = lightConstant.directionLightData.directionLightShadowMapData;

	directionLightShadowMapData.shadowDepthBias = shadowManager->getShadowDepthBias();
	directionLightShadowMapData.shadowColor = shadowManager->getShadowColor();
	directionLightShadowMapData.shadowFilterRadius = shadowManager->getShadowFilterRadius();
	directionLightShadowMapData.shadowSampleCount = shadowManager->getShadowSampleCount();


	directionLightShadowMapData.cascadeCount = shadowManager->getDirectionLightCascadeCount();
	for (int i = 0; i < directionLightShadowMapData.cascadeCount; i++)
	{
		directionLightShadowMapData.viewProjection[i] = shadowManager->getDirectionLightViewProjection(i);
		directionLightShadowMapData.cascadePlaneDistances[i] = shadowManager->getDirectionLightDistance(i + 1);
	}

}

void LightBuffer::UpdatePointLightData()
{
	ShadowManager* shadowManager = GetFrom<LightManager>(GameEngine::get()->getLightManager())->getShadowManager();
	LightManager* lightManager = GetFrom<LightManager>(GameEngine::get()->getLightManager());

	lightConstant.pointLightsSize = static_cast<int>(lightManager->getSizePointLightList());
	lightConstant.spotLightsSize = static_cast<int>(lightManager->getSizeSpotLightList());
	for (int i = 0; i < lightConstant.pointLightsSize; i++)
	{
		PointLight* pLight = lightManager->getPointLightAt(i);
		BasicElementData& elementData = lightConstant.pointLights[i].basicElementData;
		elementData.position = pLight->getPosition();
		elementData.power = pLight->getPower();
		elementData.range = pLight->getRange();
		elementData.color = pLight->getColor();
	}

	UpdatePointLightVolumetricLightData();
	UpdatePointLightShadowMapData();

}

void LightBuffer::UpdatePointLightShadowMapData()
{
	ShadowManager* shadowManager = GetFrom<LightManager>(GameEngine::get()->getLightManager())->getShadowManager();
	LightManager* lightManager = GetFrom<LightManager>(GameEngine::get()->getLightManager());


	for (size_t i = 0; i < lightManager->getSizePointLightList(); i++)
	{
		if (!lightManager->getPointLightAt(i)->getShadowCast()) continue;
		PointLightShadowMapData& pointLightShadowMapData = lightConstant.pointLights[i].shadowMapData;
		pointLightShadowMapData.shadowDepthBias = lightManager->getPointLightAt(i)->getShadowDepthBias();
		pointLightShadowMapData.shadowFilterRadius = lightManager->getPointLightAt(i)->getShadowFilterRadius();
		pointLightShadowMapData.shadowSampleCount = lightManager->getPointLightAt(i)->getShadowSampleCount();
		pointLightShadowMapData.shadowColor = lightManager->getPointLightAt(i)->getShadowColor();
		pointLightShadowMapData.slideCount = shadowManager->getPointLightCascadeCount(i);
		pointLightShadowMapData.distance = shadowManager->getPointLightDistance(i);
		for (int slide = 0; slide < pointLightShadowMapData.slideCount; slide++)
			pointLightShadowMapData.viewProjection[slide] = shadowManager->getPointLightViewProjection(i, slide);
	}


}

void LightBuffer::UpdatePointLightVolumetricLightData()
{
	ShadowManager* shadowManager = GetFrom<LightManager>(GameEngine::get()->getLightManager())->getShadowManager();
	LightManager* lightManager = GetFrom<LightManager>(GameEngine::get()->getLightManager());

	lightConstant.pointLightsSize = static_cast<int>(lightManager->getSizePointLightList());
	lightConstant.spotLightsSize = static_cast<int>(lightManager->getSizeSpotLightList());
	for (int i = 0; i < lightConstant.pointLightsSize; i++)
	{
		PointLight* pLight = lightManager->getPointLightAt(i);
		VolumetricLightScatteData& volumetricLightScatteData = lightConstant.pointLights[i].volumetricLightScatteData;
		volumetricLightScatteData.volumentricLightScatteringSample = pLight->getVolumentricLightScatteSample();
		volumetricLightScatteData.Density = pLight->getDensity();
		volumetricLightScatteData.Weight = pLight->getWeight();
		volumetricLightScatteData.Decay = pLight->getDecay();
		volumetricLightScatteData.Exposure = pLight->getExposure();
	}
}
