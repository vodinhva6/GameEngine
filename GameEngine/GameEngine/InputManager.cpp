#include "InputManager.h"
#ifdef USE_IMGUI
#include "../ImGui/imgui.h"
#endif

    using namespace DirectX;
    using namespace input;
    

    // キー割り当てデータ
    PadAssign keyAssign00[] = {
        { PAD_UP        , DirectX::Keyboard::W },            // 'W'
        { PAD_DOWN      , DirectX::Keyboard::S },            // 'S'
        { PAD_LEFT      , DirectX::Keyboard::A },            // 'A'
        { PAD_RIGHT     , DirectX::Keyboard::D },            // 'D'

        { PAD_START     , DirectX::Keyboard::Enter },        // VK_RETURN
        //{ PAD_SELECT    , Keyboard::Back },         // VK_BACK
        //{ PAD_TRG1      , Keyboard::Space },            // 'N'
        { PAD_TRG3      , DirectX::Keyboard::Escape },            // 'M'

        //{ PAD_TRG4      , 'V' },                    // 'V'

        { PAD_L1        , DirectX::Keyboard::H },        // VK_SPACE
        { PAD_R1        , DirectX::Keyboard::J },            // 
        { PAD_R2        , DirectX::Keyboard::K },            // 
        { PAD_L3        , DirectX::Keyboard::Left},            // 

        { PAD_TRG1    , VK_LBUTTON },              // マウス左ボタン
        { PAD_TRG4     , VK_RBUTTON },              // マウス右ボタン
        { PAD_R3       , VK_MBUTTON },              // マウス中ボタン
          { MOUSE_BACK       , VK_XBUTTON1 },             
        {KEY_F1 , DirectX::Keyboard::F1},
        {KEY_F2 , DirectX::Keyboard::F2},
        {KEY_F3 , DirectX::Keyboard::F3},
        {KEY_F4 , DirectX::Keyboard::F4},
        {KEY_X , DirectX::Keyboard::X},
        {KEY_Z , DirectX::Keyboard::Z},
        {KEY_SPACE, DirectX::Keyboard::Space},
        {KEY_T, DirectX::Keyboard::T},
        {KEY_S, DirectX::Keyboard::S},
        {KEY_R, DirectX::Keyboard::R},
        {KEY_DELETE, DirectX::Keyboard::Delete},
        {KEY_E, DirectX::Keyboard::E},
        {KEY_Q, DirectX::Keyboard::Q},
      
        { 0x00, 0x00 }                              // 終了コード
    };

    // ジョイスティック割り当てデータ
    PadAssign joyAssign00[] = {
        { PAD_UP        , PadGame::UP },            // 上キー
        { PAD_DOWN      , PadGame::DOWN },          // 下キー
        { PAD_LEFT    	, PadGame::LEFT },          // 左キー
        { PAD_RIGHT     , PadGame::RIGHT },         // 右キー

        //{ PAD_START     , PadGame::START },         // スタートボタン
        { PAD_TRG1      , PadGame::A },             // Aボタン
        { PAD_TRG2      , PadGame::B },             // Bボタン
        { PAD_TRG3      , PadGame::START },             // Xボタン
        { PAD_TRG4      , PadGame::Y },             // Yボタン
        { PAD_L1        , PadGame::LSHOULDER },     // Lボタン
        { PAD_R1        , PadGame::RSHOULDER },     // Rボタン

        { PAD_SELECT    , PadGame::BACK },          // バック（セレクト）ボタン

        { 0x00, 0x00 }                              // 終了コード
    };

    // キー割り当てデータ
    PadAssign keyAssign01[] = {
        //{ PAD_UP       , Keyboard::Up },            // VK_UP
        //{ PAD_DOWN     , Keyboard::Down },          // VK_DOWN
        //{ PAD_LEFT     , Keyboard::Left },          // VK_LEFT
        //{ PAD_RIGHT    , Keyboard::Right },         // VK_RIGHT

        //{ PAD_UP        , Keyboard::W },            // VK_W
        //{ PAD_DOWN      , Keyboard::S },            // VK_S
        //{ PAD_LEFT      , Keyboard::A },            // VK_A
        //{ PAD_RIGHT     , Keyboard::D },            // VK_D

        { PAD_TRG1      , DirectX::Keyboard::J },            // 'J'
        { PAD_TRG2      , DirectX::Keyboard::K },            // 'K'
        { PAD_TRG3      , DirectX::Keyboard::L },            // 'L'

        { PAD_SELECT    , DirectX::Keyboard::F1 },           // VK_F1

//      { PAD_START     , VK_LBUTTON },             // マウス左ボタン
//      { PAD_SELECT    , VK_RBUTTON },             // マウス右ボタン
//      { PAD_TRG1      , VK_MBUTTON },             // マウス中ボタン

        { 0x00, 0x00 }                              // 終了コード
    };

    //--------------------------------------------------------------
    //  キー割り当ての設定
    //--------------------------------------------------------------
    void InputEngine::setKeyAssign(int no, PadAssign *data)
    {
        pad[no].keyAssign = data;
    }

    //--------------------------------------------------------------
    //  ジョイスティック割り当ての設定
    //--------------------------------------------------------------
    void InputEngine::setJoyAssign(int no, PadAssign *data)
    {
        pad[no].joyAssign = data;
    }

    InputEngine::~InputEngine()
    {
       

    }


    void InputEngine::Init(const HWND* hwnd)
    {
        
        HRESULT hr = S_OK;
        keyboard = std::make_unique<Keyboard>();
        gamePad = std::make_unique<GamePad>(); 

        mousePoint.reset(new MousePoint(hwnd));
        controlPad.reset(new ControlPad);


        SecureZeroMemory(pad, sizeof(PadState) * input::GAMEPAD_NUM);

        setKeyAssign(0, keyAssign00);
        setJoyAssign(0, joyAssign00);


        setKeyAssign(1, keyAssign01);
        setJoyAssign(1, joyAssign00);


        setJoyAssign(2, joyAssign00);


        setJoyAssign(3, joyAssign00);
    }

    void InputEngine::Update()
    {

        DirectX::Keyboard::State kb;
        kb = keyboard->GetState();
        DirectX::GamePad::State gpad[input::GAMEPAD_NUM] = {};

        for (int i = 0; i < input::GAMEPAD_NUM; i++)
            gpad[i] = gamePad->GetState(i);

        // 
        for (int i = 0; i < GAMEPAD_NUM; i++)
        {
            PadState* p = &pad[i];

            int old_state = p->state;


            p->state = 0;


            PadAssign *assign = p->keyAssign;
            if (assign)
            {
                while (assign->bit)
                {
                    if ((assign->code == VK_LBUTTON) ||
                        (assign->code == VK_RBUTTON) ||
                        (assign->code == VK_MBUTTON) || 
                        (assign->code == VK_XBUTTON1))
                    {	// Mouse
                        
                        SHORT c = GetAsyncKeyState(assign->code);
                        if (c < 0)
                            p->state |= assign->bit;
                    }
                    else
                    {	// Keyboard
                        if (kb.IsKeyDown((Keyboard::Keys)assign->code))
                            p->state |= assign->bit;
                    }

                    assign++;
                }
            }


            assign = p->joyAssign;
            if (assign && gpad[i].IsConnected())
            {
                while (assign->bit)
                {
                    struct hoge_t { bool b[10]; } hoge;
                    if (assign->code >= PadGame::A)
                    {	// buttons
                        hoge = *(hoge_t*)&gpad[i].buttons;
                        if (hoge.b[assign->code - PadGame::A]) p->state |= assign->bit;
                    }
                    else
                    {	// dpad
                        hoge = *(hoge_t*)&gpad[i].dpad;
                        if (hoge.b[assign->code]) p->state |= assign->bit;
                    }

                    assign++;
                }

                // 左スティック
                p->leftX = gpad[i].thumbSticks.leftX;
                p->leftY = gpad[i].thumbSticks.leftY;

                // 右スティック
                p->rightX = gpad[i].thumbSticks.rightX;
                p->rightY = gpad[i].thumbSticks.rightY;

                // 左右トリガー
                p->left = gpad[i].triggers.left;
                p->right = gpad[i].triggers.right;


            }

            // p->triggerの更新
            p->trigger = (~old_state) & p->state;

            // p->triggerUpの更新
            p->triggerUp = old_state & (~p->state);

            // p->repeatの計算
            p->repeat = p->state;
            for (int j = 8 - 1; j > 0; --j) {
                p->old[j] = p->old[j - 1];				
                p->repeat &= p->old[j];					
            }
            p->old[0] = p->state & (~p->repeat);		
            p->repeat |= p->trigger;					
        }

        mousePoint.get()->Update();
        controlPad.get()->Update();


        


    }


    void InputEngine::setVibration(int PlayerNum, float motorLeft, float motorRight)
    {
        gamePad->SetVibration(PlayerNum, motorLeft, motorRight);
    }



//******************************************************************************
