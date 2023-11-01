#pragma once
#include <DirectXMath.h>
class ShadowManager
{
public:
    ShadowManager();
	void Init();
	void Update();
    UINT getDirectionLightCascadeCount() { return directShadowMapInfor.cascadeCount; }
    DirectX::XMFLOAT4X4& getDirectionLightViewProjection(size_t num) { return directShadowMapInfor.viewProjections.at(num); }
    DirectX::XMFLOAT4X4& getPointLightViewProjection(size_t slot, size_t num) { return pointLightsShadowMapInfor[slot].viewProjections.at(num); }
    float getDirectionLightDistance(size_t num) { return directShadowMapInfor.distances.at(num); }
    float getPointLightDistance(size_t slot) { return pointLightsShadowMapInfor[slot].distances; }
    UINT getPointLightCascadeCount(size_t slot) { return pointLightsShadowMapInfor[slot].slideCount; }

    float getShadowDepthBias() { return directShadowMapInfor.shadowDepthBias; }
    float getShadowColor() { return directShadowMapInfor.shadowColor; }
    float getShadowFilterRadius() { return directShadowMapInfor.shadowFilterRadius; }
    int getShadowSampleCount() { return directShadowMapInfor.shadowSampleCount; }

    ~ShadowManager();


public:
    template < class T>
    void serialize(T& archive)
    {
        archive(directShadowMapInfor);
    }

private:
	void UpdateProjection();
    void CreateDirectionLightShadowMap();
    void CreatePointLightsShadowMap();
private:
    struct DirectionLightShadowMapInfor
    {
        std::vector<DirectX::XMFLOAT4X4> viewProjections;
        std::vector<float> distances;

        float shadowDepthBias;
        float shadowColor;
        float shadowFilterRadius;

        int shadowSampleCount;

        UINT cascadeCount;
        float splitCchemeWeight;
    public:
        template < class T>
        void serialize(T& archive)
        {
            archive(viewProjections, distances, shadowDepthBias, shadowColor, shadowFilterRadius,
                shadowSampleCount, cascadeCount, splitCchemeWeight);
        }

    } directShadowMapInfor;
    
    struct PointLightsShadowMapInfor
    {
        std::vector<DirectX::XMFLOAT4X4> viewProjections;
        float distances;

        UINT slideCount;



    };
    PointLightsShadowMapInfor pointLightsShadowMapInfor[10];
    
};

