#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H
#include "OBJ3D.h"
#include "StateMachine.h"
class Character : public OBJ3D
{
public:
    
    Character();
    Character(const OBJ3D& obj) : OBJ3D(obj) {}
    virtual OBJ3DType getTypeOBJ3D() override { return OBJ3DType::CHARATER; }
    virtual void Update(float elapsedTime) override;
    virtual void Draw() override;
    virtual void UpdateAnimation(float elapsedTime) override;
    virtual void SaveKeyAnimation(float elapsedTime) override;
    virtual void UpdateMove(float value, float elapsedTime);
    virtual  ~Character() override;
    void SetAnimation(std::string clip, std::string sub);
    void impluseMoveSpeed(float elapsed_time);
    virtual void BlendAnimation(float elapsedTime);
    std::shared_ptr<StateMachine>& getStateMachine() { return stateMachine; }
private:
    void VerticalUpdate(float elapsed_time);
    void HorizoncalUpdate(float elapsed_time);
   
    void GravityImpuse(float elapsed_time);

public:
    bool GetBlending() { return blending; }
    void SetBlending(bool blend) { blending = blend; }

    void BeginBlendingAnimation(float blenderTimeStep);


    VECTOR3 getVectorForward();
    int getFrameAnimationNow() { return frameAnimationNow; }
    int getFrameAnimationOld() { return frameAnimationOld; }
protected:
    std::string animationClip;
    std::string animationClipSub;
    std::string animationClipOld;
    std::string animationClipSubOld;
    bool onGround;
    bool blending;
    float blendTime;
    int frameAnimationNow;
    int frameAnimationOld;
    float blenderTimeStep;
protected:
    std::shared_ptr<StateMachine> stateMachine;
};


#endif // !CHARACTER_H