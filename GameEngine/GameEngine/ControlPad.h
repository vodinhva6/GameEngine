#pragma once
#include "DataFormat.h"
#define MAX_PLAYER  2
enum class Controler
{
    KEYBOARD,
    GAMEPAD
};
class ControlPad
{
public:
    ControlPad();
    VECTOR2 getPosLeftJoy(int playerNum);
    VECTOR2 getPosRightJoy(int playerNum);

    VECTOR2 getOldPosLeftJoy(int playerNum);
    VECTOR2 getOldPosRightJoy(int playerNum);


    float getTriggerRight(int playerNum);
    bool PressT(int PlayerNum);
    bool PressS(int PlayerNum);
    bool PressE(int PlayerNum);
    bool PressQ(int PlayerNum);
    bool PressR(int PlayerNum);
    bool PressDelete(int PlayerNum);

    bool PressA(int PlayerNum);
    bool PressRightMouse(int PlayerNum);
    bool HoldeA(int playerNum);
    bool PressF1(int playerNum);
    bool PressF2(int playerNum);
    bool PressF3(int playerNum);
    bool PressF4(int playerNum);
    bool HoldeZ(int playerNum);
    bool HoldeX(int playerNum);
    bool PressSpace(int playerNum);

    bool PressLeft(int PlayerNum);
    bool PressRight(int PlayerNum);
    bool PressUp(int PlayerNum);
    bool PressDown(int PlayerNum);
    bool PressStart(int PlayerNum);
    bool PressBack(int PlayerNum);
    bool PressY(int PlayerNum);


    bool ReleaseA(int PlayerNum);


    void setVibration(int PlayerNum, float motorLeft, float motorRight);
    void setEnable(bool ena) { enableUsing = ena; }


    void SetControllerType(Controler type) { controler_ = type; }
    void Update();
private:
    int TRG(int playerNum);
    int STATE(int playerNum);
    

    struct PlayerNum
    {
        VECTOR2 PositionLeftJoy;
        VECTOR2 PositionRightJoy;

        VECTOR2 OldPositionLeftJoy;
        VECTOR2 OldPositionRightJoy;

        float TriggerRight;
        int PressTRG;
        int Release;
        int StatePress;

    };
    PlayerNum playerNum_[MAX_PLAYER];
    Controler controler_;
    bool enableUsing;
};

