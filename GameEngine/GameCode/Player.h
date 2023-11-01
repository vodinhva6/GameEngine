#pragma once
#include "Charater.h"

class Player :public Character
{
public:
    Player();
    Player(const OBJ3D& obj);
    virtual OBJ3DType getTypeOBJ3D() override { return OBJ3DType::PLAYER; }
    void Update(float elapsedTime) override;
    void Draw() override;
    void UpdateMove(float value, float elapsed_time) override;
    template < class T>
    void serialize(T& archive)
    {
        archive(cereal::base_class<OBJ3D>(this), MAXSpeed);
    }
    ~Player() override;
    int runUptimer = 0;
private:
    
    float moveSpeed;
    float MAXSpeed;
    float animationSpeed;
};

CEREAL_REGISTER_TYPE(Player);
