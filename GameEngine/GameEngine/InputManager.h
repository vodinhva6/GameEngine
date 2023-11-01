#ifndef INCLUDED_INPUT_MANAGER
#define	INCLUDED_INPUT_MANAGER

#include <Windows.h>
#include <Keyboard.h>
#include <GamePad.h>
#include "MousePoint.h"
#include "ControlPad.h"
#include "SingletonPatternTemplate.h"
class ControlPad;
namespace DirectX
{
    namespace input
    {

        const int PAD_UP = (1 << 0);
        const int PAD_DOWN = (1 << 1);
        const int PAD_LEFT = (1 << 2);
        const int PAD_RIGHT = (1 << 3);
        const int PAD_START = (1 << 4);
        const int PAD_TRG1 = (1 << 5);
        const int PAD_TRG2 = (1 << 6);
        const int PAD_TRG3 = (1 << 7);
        const int PAD_SELECT = (1 << 8);
        const int PAD_TRG4 = (1 << 9);
        const int PAD_L1 = (1 << 10);
        const int PAD_R1 = (1 << 11);
        const int PAD_L2 = (1 << 12);
        const int PAD_R2 = (1 << 13);
        const int PAD_L3 = (1 << 14);
        const int PAD_R3 = (1 << 15);
        const int KEY_F1 = (1 << 16);
        const int KEY_F2 = (1 << 17);
        const int KEY_Z = (1 << 18);
        const int KEY_X = (1 << 19);
        const int KEY_SPACE = (1 << 20);
        const int KEY_T = (1 << 21);
        const int KEY_S = (1 << 22);
        const int KEY_R = (1 << 23);
        const int KEY_DELETE = (1 << 24);
        const int KEY_F3 = (1 << 25);
        const int KEY_E = (1 << 26);
        const int KEY_Q = (1 << 27);
        const int KEY_F4 = (1 << 28);
        const int MOUSE_BACK = (1 << 15);

        const int	GAMEPAD_NUM = 4;

        enum PadGame
        {
            UP = 0,
            DOWN,
            RIGHT,
            LEFT,
            A = 4,
            B,
            X,
            Y,
            LSTICK,
            RSTICK,
            LSHOULDER,
            RSHOULDER,
            BACK,
            START,
        };

    }

    struct PadAssign
    {
        int         bit;
        int         code;
    };

    class PadState
    {
    public:

        int         state;
        int         trigger;
        int         triggerUp;
        int         repeat;
        int         old[8];
        float       leftX, leftY;
        float       rightX, rightY;
        float       left, right;
        PadAssign* keyAssign;
        PadAssign* joyAssign;
    };
}
    class InputEngine : public SingletonPatternTemplate<InputEngine>
    {
        friend class SingletonPatternTemplate<InputEngine>;
    private:
        std::unique_ptr<DirectX::Keyboard>  keyboard ;   
        std::unique_ptr<DirectX::GamePad> gamePad    ;

        std::unique_ptr<MousePoint> mousePoint;
        std::unique_ptr<ControlPad> controlPad;


        DirectX::PadState     pad[DirectX::input::GAMEPAD_NUM] = {};

        
        void setKeyAssign(int no, DirectX::PadAssign* data); 
        void setJoyAssign(int no, DirectX::PadAssign* data); 

    public:
        InputEngine() {}
        ~InputEngine();
       
        void Init(const HWND* hwnd);                                

        MousePoint* getMousePoint() { return mousePoint.get(); }
        ControlPad* getControlPad() { return controlPad.get(); }

        void Update();                         

        DirectX::PadState* getPadAddress() { return pad; }

        bool PadGameConnected(int playerNum) { return gamePad->GetState(playerNum).IsConnected(); }

       
        void setVibration(int PlayerNum, float motorLeft, float motorRight);
    };



#endif // !INCLUDED_INPUT_MANAGER
