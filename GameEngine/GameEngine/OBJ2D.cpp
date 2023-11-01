//#include "Scene.h"
#include "OBJ2D.h"
#include "MyMath.h"
#include "GameEngine.h"
OBJ2D::OBJ2D()
{
    spr_ = nullptr;
    size_ = 1;
}

void OBJ2D::Update(float elapsedTime)
{
}

void OBJ2D::Draw()
{

    GameEngine::get()->DrawSpriteLate((void*)spr_, position_, scale_, color_, texturePosition_, textureSize_, 0, 10);
}

OBJ2D::~OBJ2D()
{

}

