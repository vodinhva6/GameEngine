#include "Charater.h"
#include "Collision.h"
Character::Character()
{
    onGround = false;
    blendTime = 0;
}

void Character::Update(float elapsedTime)
{    
    stateMachine->Run(elapsedTime);
   
}

void Character::UpdateAnimation(float elapsedTime)
{
    if (blending)
        BlendAnimation(elapsedTime);
    else  SaveKeyAnimation(elapsedTime);
}

void Character::SaveKeyAnimation(float elapsedTime)
{
     if (blendTime != 0.0f) return;
    animation.timeLine += elapsedTime * 1;
    if (animation.timeLine > 1)
        animation.timeLine -= (int)animation.timeLine;

    if (meshInfor.animator_.get())
    {
        frameAnimationOld = frameAnimationNow;
        animation.key = meshInfor.animator_->GetFeyframeAt(
            animationClip, animationClipSub, animation.timeLine, frameAnimationNow);
    }
}

void Character::SetAnimation(std::string clip, std::string sub)
{
    animationClipOld = animationClip;
    animationClipSubOld = animationClipSub;
    animationClip = clip;
    animationClipSub = sub;
}

void Character::BlendAnimation(float elapsedTime)
{
    
    frameAnimationOld = frameAnimationNow;
    animation.key = meshInfor.animator_->BlendAnimations(animationClipOld,animationClipSubOld, animation.timeLine,
        animationClip,animationClipSub, blendTime, frameAnimationNow);
    blendTime += blenderTimeStep;
    if (blendTime >= 1)
    {
        animationClipOld = animationClip;
        animationClipSubOld = animationClipSub;
        blendTime = 0;
        blending = false;
        animation.timeLine = meshInfor.animator_->CalTimeLine(animationClip, animationClipSub, frameAnimationNow);
        return;
    }
    blending = true;
}

Character::~Character()
{
}




void Character::Draw()
{

}

void Character::UpdateMove(float value, float elapsedTime)
{
}

void Character::GravityImpuse(float elapsed_time)
{
    static float gravityRate = 0.06f;
#ifdef USE_IMGUI
    ImGui::SliderFloat("gravityRate", &gravityRate, 0, 0.2f);
#endif
    speed_.y += gravityValue;
    if (speed_.y < 0)
        speed_.y += speed_.y * gravityRate;
}

void Character::BeginBlendingAnimation(float blenderTimeStep)
{
    blendTime = 0;
    blending = true;
    this->blenderTimeStep = blenderTimeStep;
}

VECTOR3 Character::getVectorForward()
{
    DirectX::XMFLOAT4X4 rot;
    DirectX::XMStoreFloat4x4(&rot, DirectX::XMMatrixRotationRollPitchYaw(MyMath::get()->convertToRadian(rotation_.x)
        , MyMath::get()->convertToRadian(rotation_.y), MyMath::get()->convertToRadian(rotation_.z)));
    return { rot._31,rot._32 ,rot._33 };
}

void Character::VerticalUpdate(float elapsed_time)
{

}

void Character::HorizoncalUpdate(float elapsed_time)
{

}


void Character::impluseMoveSpeed(float elapsed_time)
{
    position_ += speed_;
    rotation_ += speedRotation_;
    speedRotation_ *= 0.8f;
    speed_ *= 0.65f;
}
