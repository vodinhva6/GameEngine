#pragma once
#include "OBJ2D.h"

class MousePoint 
{
public:

    MousePoint(const HWND* hwnd);
    void Update();
    void draw();
    void setStopSetWheelValue(bool set) { stopSetWheelValue = set; }
    void setEnable(bool ena) { enableUsing = ena; }
    void resetPosition();
    void setWheelValue(float value) { wheelValue = value; }
    void setWheel(float wheel) { wheel_ = wheel; }
    ~MousePoint();


public:
    bool onDoubleClick();
    bool onClickLeft();
    bool onClickRight();
    bool getStopWheelValue() { return stopSetWheelValue; }
    bool onTrigger(OBJ2D&);
    bool onTriggerNoMoveEnble(OBJ2D&);
    bool onClickTrigger(OBJ2D&);
    bool onMove();
    bool BackButtonClick();

public:
    float getCulPos()
    {
        return mousePosition.x - oldPos.x;
    }
    float getWheelValue() { return wheelValue; }
public:
   
    VECTOR2 getPos()
    {
        return mousePosition;
    }
    VECTOR2 getOldPos()
    {
        return oldPos;
    }
    POINT getPosLong()
    {
        return point_;
    }
    VECTOR2 getIns()
    {
        return mousePosition - oldPos;
    }
    const HWND* hwnd;
private:
    struct MousePointInfor
    {
        bool clickLeft;
        bool clickRight;
        bool clickMiddle;
        bool clickBack;
        bool clickForward;

    };
    MousePointInfor mouseInfor;
    void UpdateMouseInfor();
   
protected:
    void init(const HWND* hwnd);
    void NotUsingMouseNow();
    void resetCountDown();
    void wheelSet();

    bool mouseInHere(const VECTOR2& mousPos, OBJ2D& here);
protected:
    int notUingMouseTimer;
    int timer;
    bool enableUsing;
    bool smooth;
    bool stopSetWheelValue;

    float oldWheel_;
    float wheel_;
    float wheelValue;

protected:
    VECTOR2 mousePosition;
    Sprite* mouse;
    VECTOR2 oldPos;
    POINT point_;
};
