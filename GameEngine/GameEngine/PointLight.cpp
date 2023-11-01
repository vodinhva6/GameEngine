#include "LightManager.h"
#include "FramebufferNumName.h"
#include "GameEngine.h"

PointLight::PointLight(const VECTOR3& pos, const VECTOR4& colr, const float& rag, const float& pow):OBJ3D()
{
    position_.x = pos.x;
    position_.y = pos.y;
    position_.z = pos.z;

    color_ = colr;

    basicElementData.range = rag;
    basicElementData.power = pow;
    drawStates_.drawType = DrawType::MeshLightWithEmissionShader;
    name_ = "PointLight";
}

void PointLight::Init()
{
}

void PointLight::Update(float elapsedTime)
{
    updateWorldTrans();
    Actor::Update(elapsedTime);
}

void PointLight::DrawSphere(ID3D11DeviceContext* dc)
{
    if (!volumetricLightScatteData.renderWithSphere) return;
    if (meshInfor.mesh_.get())
        meshInfor.mesh_->Render(dc, worldTransform, meshInfor.materials_, color_);
}

void PointLight::DrawDebug()
{
    if (sprite3D_.get())
        GameEngine::get()->DrawSprite3D(SetToPoint(sprite3D_), worldTransform, color_, drawTurn_, FrameBufferName::MSAABUFFER1);
}

PointLight::~PointLight()
{

}
