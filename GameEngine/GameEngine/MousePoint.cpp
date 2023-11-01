#include "Audio.h"
#include "InputManager.h"
MousePoint::MousePoint(const HWND* hwnd)
{
    init(hwnd);
    //mouse = GraphicEngine::get()->createSprite(L"./Data/Asset/Images/Mouse/cursor.png");
    timer = 0;
    wheel_ = 0;
    notUingMouseTimer = 300;
}

void MousePoint::Update()
{
   
    NotUsingMouseNow();


    GetCursorPos(&point_);
    ScreenToClient(*hwnd, &point_);
    UpdateMouseInfor();

    oldPos = mousePosition;
    mousePosition = { static_cast<float>(point_.x),static_cast<float>(point_.y) };
    wheelSet();
    oldWheel_ = 0;

    timer++;
}


void MousePoint::UpdateMouseInfor()
{
    InputEngine* inputMana = InputEngine::get();
    mouseInfor.clickLeft = inputMana->getControlPad()->PressA(0);
    mouseInfor.clickRight = inputMana->getControlPad()->PressRightMouse(0);
    mouseInfor.clickBack = inputMana->getPadAddress()->trigger & DirectX::input::MOUSE_BACK;
}

void MousePoint::init(const HWND* hwnd)
{
    this->hwnd = hwnd;
}

bool MousePoint::onDoubleClick()
{
    static int firtFlag = -1;
    bool a = mouseInfor.clickLeft;
    if (a)
    {
        if (firtFlag == -1)
        {
            firtFlag = timer;
            a = false;
        } else
        {
            a = (timer - firtFlag < 60);
            firtFlag = -1;
           
        }
    }

    
    return a;
}

bool MousePoint::onClickLeft()
{
    bool a = mouseInfor.clickLeft;
    if (a)
    {
        resetCountDown();
        //AudioManager::get()->play(EFFECT, EFFECT_SELECT);
    }
    return a;
}

bool MousePoint::onClickRight()
{
    bool a = mouseInfor.clickRight;
    if (a)
    {
        resetCountDown();
        //AudioManager::get()->play(EFFECT, EFFECT_SELECT);
    }
    return a;
}

void MousePoint::draw()
{
    
#if _DEBUG

#else
    VECTOR2 offset = { 32,17 };
    //GraphicEngine::get()->drawSpriteLate(mouse, mousePosition - offset, { 1.0f,1.0f }, { 1,1,1,1 }, { 0,0 }, { 100,100 }, 0, 10);
#endif // !_DEBUG

}
bool MousePoint::onTrigger(OBJ2D& here)
{
    if (oldPos == mousePosition)
        return false;
    return mouseInHere(mousePosition, here);
}

bool MousePoint::onTriggerNoMoveEnble(OBJ2D& here)
{
    return mouseInHere(mousePosition, here);
}

bool MousePoint::onClickTrigger(OBJ2D& here)
{
    if (mouseInfor.clickLeft)
    {
        return mouseInHere(mousePosition, here);
        resetCountDown();
    }
        
    return false;
}

bool MousePoint::onMove()
{
    float dx = oldPos.x - mousePosition.x;
    float dy = oldPos.y - mousePosition.y;
    return (fabsf(dx) > 1.0f || fabsf(dy) > 10.0f );
}

bool MousePoint::BackButtonClick()
{
    return mouseInfor.clickBack;
}


void MousePoint::resetPosition()
{
    SetCursorPos(1920 / 2, 1080 / 2);
    GetCursorPos(&point_);
    ScreenToClient(*hwnd, &point_);
    mousePosition = { static_cast<float>(point_.x),static_cast<float>(point_.y) };
    oldPos = mousePosition;
}




MousePoint::~MousePoint()
{

}

void MousePoint::NotUsingMouseNow()
{
    if (oldPos == mousePosition)
        notUingMouseTimer--;
    else
    {
        resetCountDown();
    }
    if (notUingMouseTimer < 0)
        notUingMouseTimer = 0;
}

void MousePoint::resetCountDown()
{
    notUingMouseTimer = 300;
}

bool MousePoint::mouseInHere(const VECTOR2& mousPos, OBJ2D& here)
{
    VECTOR2 pos;
    pos = here.getPosition();
    VECTOR2 hsize = {
        (fabsf(here.getHSizeMin().x) + fabsf(here.getHSizeMax().x)) / 2,
        (fabsf(here.getHSizeMin().y) + fabsf(here.getHSizeMax().y)) / 2 };
    if (mousPos.x > pos.x && mousPos.x < pos.x + hsize.x)
        if (mousPos.y > pos.y && mousPos.y < pos.y + hsize.y)
            return true;
    return false;
}

void MousePoint::wheelSet()
{
    //ControlPad* conPad = ControlPad::get();
    //bool usingPad = false;
    //float subValue = 0;
   ///* if (conPad->getUsingControler())
    //    usingPad = true;*/
    //if (usingPad)
    //{
    //    float rightJoyY = conPad->getPosRightJoy(0).y;
    //    wheelValue += rightJoyY;
    //    if (rightJoyY == 0)
    //        subValue = 0.8f;
    //    else
    //    {
    //        subValue = 0.1f * rightJoyY;
    //    }
    //}
    //else
    //{
    //    if (wheel_ - oldWheel_ < 0)
    //    {
    //        wheelValue += -15;
    //        if (wheelValue < -30)
    //            wheelValue = -30;
    //    }
    //    else
    //        if (wheel_ - oldWheel_ > 0)
    //        {
    //            wheelValue += 15;
    //            if (wheelValue > 30)
    //                wheelValue = 30;
    //        }
    //    subValue = 1;
    //}
    //if (stopSetWheelValue)
    //    wheelValue = 0;
    //if (wheelValue < 0)
    //{
    //    wheelValue += subValue;
    //    if (wheelValue > 0)
    //        wheelValue = 0;
    //}
    //else
    //    if (wheelValue > 0)
    //    {
    //        wheelValue -= subValue;
    //        if (wheelValue < 0)
    //            wheelValue = 0;
    //    }
    //
    //stopSetWheelValue = false;
   
}
