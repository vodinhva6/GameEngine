#include "ControlPad.h"
#include "InputManager.h"
#include "MyMath.h"
using namespace DirectX;
using namespace input;
VECTOR2 ControlPad::getPosLeftJoy(int playerNum)
{
    return playerNum_[playerNum].PositionLeftJoy;
}

VECTOR2 ControlPad::getPosRightJoy(int playerNum)
{
    return playerNum_[playerNum].PositionRightJoy;
}

VECTOR2 ControlPad::getOldPosLeftJoy(int playerNum)
{
    return playerNum_[playerNum].OldPositionLeftJoy;
}

VECTOR2 ControlPad::getOldPosRightJoy(int playerNum)
{
    return playerNum_[playerNum].OldPositionRightJoy;
}

float ControlPad::getTriggerRight(int playerNum)
{
    if (!enableUsing)
        return false;
    return playerNum_[playerNum].triggerRight;
    
}

float ControlPad::getTriggerLeft(int playerNum)
{
    if (!enableUsing)
        return false;
    return playerNum_[playerNum].triggerLeft;
}

bool ControlPad::PressT(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & KEY_T);
}

bool ControlPad::PressS(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & KEY_S);
}

bool ControlPad::PressE(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & KEY_E);
}

bool ControlPad::PressQ(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & KEY_Q);
}

bool ControlPad::PressR(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & KEY_R);
}

bool ControlPad::PressDelete(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & KEY_DELETE);
}

int ControlPad::TRG(int playerNum)
{
    if (!enableUsing)
        return false;
    return true;// InputManager::get()->getPadAddress()[playerNum].trigger;
}

int ControlPad::STATE(int playerNum)
{
    if (!enableUsing)
        return false;
    return true;//InputManager::get()->getPadAddress()[playerNum].state;
}

bool ControlPad::PressA(int PlayerNum)
{
    if (!enableUsing)
        return false;
     return (playerNum_[PlayerNum].PressTRG & PAD_TRG1);
}

bool ControlPad::PressRightMouse(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & PAD_TRG4);
}

bool ControlPad::HoldeA(int playerNum)
{
    if (!enableUsing)
        return false;
    return playerNum_[playerNum].StatePress & PAD_TRG1;
}

bool ControlPad::PressF1(int playerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[playerNum].PressTRG & KEY_F1);
}

bool ControlPad::PressF2(int playerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[playerNum].PressTRG & KEY_F2);
}

bool ControlPad::PressF3(int playerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[playerNum].PressTRG & KEY_F3);
}

bool ControlPad::PressF4(int playerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[playerNum].PressTRG & KEY_F4);
}

bool ControlPad::HoldeZ(int playerNum)
{
    if (!enableUsing)
        return false;
    return playerNum_[playerNum].StatePress & KEY_Z;
}

bool ControlPad::HoldeX(int playerNum)
{
    if (!enableUsing)
        return false;
    return playerNum_[playerNum].StatePress & KEY_X;
}

bool ControlPad::PressSpace(int playerNum)
{
    if (!enableUsing)
        return false;
    return playerNum_[playerNum].PressTRG & KEY_SPACE;
}

bool ControlPad::PressLeft(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & PAD_LEFT);
}

bool ControlPad::PressRight(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & PAD_RIGHT);
}

bool ControlPad::PressUp(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & PAD_UP);
}

bool ControlPad::PressDown(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & PAD_DOWN);
}

bool ControlPad::PressStart(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & PAD_START);
}

bool ControlPad::PressBack(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & PAD_TRG3);
}

bool ControlPad::PressY(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].PressTRG & PAD_TRG4);
}

bool ControlPad::ReleaseA(int PlayerNum)
{
    if (!enableUsing)
        return false;
    return (playerNum_[PlayerNum].Release & PAD_TRG1);
}

void ControlPad::setVibration(int PlayerNum, float motorLeft, float motorRight)
{
    InputEngine* inputMana = InputEngine::get();
    inputMana->setVibration(PlayerNum, motorLeft, motorRight);
}



void ControlPad::Update()
{

    InputEngine* inputMana = InputEngine::get();
    if (controler_ == Controler::GAMEPAD)
    {
        for (int i = 0; i < MAX_PLAYER; i++)
        {
            playerNum_[i].OldPositionLeftJoy = playerNum_[i].PositionLeftJoy;
            playerNum_[i].PositionLeftJoy.x = inputMana->getPadAddress()[i].leftX;
            playerNum_[i].PositionLeftJoy.y = inputMana->getPadAddress()[i].leftY;
    
            playerNum_[i].OldPositionRightJoy = playerNum_[i].PositionRightJoy;
            playerNum_[i].PositionRightJoy.x = inputMana->getPadAddress()[i].rightX;
            playerNum_[i].PositionRightJoy.y = inputMana->getPadAddress()[i].rightY;
    
            playerNum_[i].triggerRight = inputMana->getPadAddress()[i].right;
            playerNum_[i].triggerLeft = inputMana->getPadAddress()[i].left;
            playerNum_[i].PressTRG = inputMana->getPadAddress()[i].trigger;
            playerNum_[i].Release = inputMana->getPadAddress()[i].triggerUp;
        }
    }
    
    else
    {
        for (int i = 0; i < MAX_PLAYER; i++)
        {
            playerNum_[i].OldPositionLeftJoy = playerNum_[i].PositionLeftJoy;
            playerNum_[i].PositionLeftJoy.x = 0;
            playerNum_[i].PositionLeftJoy.y = 0;
            playerNum_[i].OldPositionRightJoy = playerNum_[i].PositionRightJoy;

            VECTOR2 Vec = MyMath::get()->Normalize(inputMana->getMousePoint()->getIns());
            playerNum_[i].PositionRightJoy.x = Vec.x;
            playerNum_[i].PositionRightJoy.y = Vec.y;

            playerNum_[i].triggerRight = 0;
            playerNum_[i].PressTRG = 0;
            int stateInput = inputMana->getPadAddress()[i].state;
            playerNum_[i].StatePress = stateInput;
            switch (stateInput & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT))
            {
            case (PAD_UP | PAD_LEFT):
                playerNum_[i].PositionLeftJoy.y = 0.7f;
                playerNum_[i].PositionLeftJoy.x = -0.7f;
                break;
            case (PAD_UP | PAD_RIGHT):
                playerNum_[i].PositionLeftJoy.y = 0.7f;
                playerNum_[i].PositionLeftJoy.x = 0.7f;
                break;
            case (PAD_DOWN | PAD_LEFT):
                playerNum_[i].PositionLeftJoy.y = -0.7f;
                playerNum_[i].PositionLeftJoy.x = -0.7f;
                break;
            case (PAD_DOWN | PAD_RIGHT):
                playerNum_[i].PositionLeftJoy.y = -0.7f;
                playerNum_[i].PositionLeftJoy.x = 0.7f;
                break;
            case PAD_UP:
                playerNum_[i].PositionLeftJoy.y = 1;
                break;
            case PAD_DOWN:
                playerNum_[i].PositionLeftJoy.y = -1;
                break;
            case PAD_LEFT:
                playerNum_[i].PositionLeftJoy.x = -1;
                break;
            case PAD_RIGHT:
                playerNum_[i].PositionLeftJoy.x = 1;
                break;

            }
            if (stateInput & PAD_TRG1)
                playerNum_[i].triggerRight = 1;
            playerNum_[i].Release = inputMana->getPadAddress()[i].triggerUp;
            playerNum_[i].PressTRG = inputMana->getPadAddress()[i].trigger;
        }
    }


}

ControlPad::ControlPad()
{
    enableUsing = true;
}

