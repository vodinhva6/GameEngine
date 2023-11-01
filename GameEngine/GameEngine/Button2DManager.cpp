//#include "Button2DManager.h"
//#include "GameEngine.h"
//
//Button2DManager::~Button2DManager()
//{
//	buttons2D.clear();
//}
//
//void Button2DManager::update()
//{
//	//ControlPad* controlPad = ControlPad::get();
//	//if (controlPad->PressLeft(0) ||
//	//	controlPad->PressRight(0) ||
//	//	controlPad->PressUp(0) ||
//	//	controlPad->PressDown(0))
//	//{
//	//	isMouse = false;
//	//}
//
//	for (auto& button2D : buttons2D)
//	{
//		button2D.update(isMouse);
//	}
//
//	if (!isMouse)
//	{
//		switch (initType)
//		{
//		case INIT_TYPE::TITLE:
//			//if (controlPad->PressLeft(0))
//			//{
//			//	--selectButtonIndex;
//			//}
//			//else if (controlPad->PressRight(0))
//			//{
//			//	++selectButtonIndex;
//			//}
//			//else if (controlPad->PressStart(0))
//			//{
//			//	buttons2D[selectButtonIndex].decision();
//			//}
//			//break;
//		case INIT_TYPE::GAME:
//			//if (controlPad->PressUp(0))
//			//{
//			//	--selectButtonIndex;
//			//}
//			//else if (controlPad->PressDown(0))
//			//{
//			//	++selectButtonIndex;
//			//}
//			//else if (controlPad->PressStart(0))
//			//{
//			//	buttons2D[selectButtonIndex].decision();
//			//}
//			break;
//		default:
//			break;
//		}
//
//		selectButtonIndex = (std::min)((std::max)(selectButtonIndex, 0), static_cast<int>(buttons2D.size() - 1));
//
//		for (int i = 0; i < buttons2D.size(); i++)
//		{
//			VECTOR4 color = { 0.5f, 0.5f, 0.5f, 1.0f };
//
//			if (i == selectButtonIndex)
//			{
//				color = { 1, 1 ,1, 1 };
//			}
//
//			buttons2D[i].setColor(color);
//		}
//	}
//}
//
//void Button2DManager::draw()
//{
//	for (auto& button2D : buttons2D)
//	{
//		button2D.Draw();
//	}
//}
//
//void Button2DManager::init(INIT_TYPE initType)
//{
//	buttons2D.clear();
//	selectButtonIndex = 0;
//	isMouse = true;
//	this->initType = initType;
//
//	switch (initType)
//	{
//	case INIT_TYPE::TITLE:
//		buttons2D.push_back(Button2D(Button2D::MY_TYPE::GO_SELECT, VECTOR2(860, 800)));
//		buttons2D.push_back(Button2D(Button2D::MY_TYPE::EXIT_GAME, VECTOR2(1400, 800)));
//		break;
//	case INIT_TYPE::GAME:
//		buttons2D.push_back(Button2D(Button2D::MY_TYPE::BACK_GAME, VECTOR2(750, 250)));
//		buttons2D.push_back(Button2D(Button2D::MY_TYPE::RETRY,	   VECTOR2(750, 450)));
//		buttons2D.push_back(Button2D(Button2D::MY_TYPE::GO_TITLE,  VECTOR2(750, 650)));
//		break;
//	default:
//		break;
//	}
//}
//
//void Button2DManager::clear()
//{
//	
//	buttons2D.clear();
//}
//
//Button2D::Button2D(MY_TYPE myType, VECTOR2 pos)
//{
//	//GraphicEngine* graphicEngine = GameEngine::get()->getGraphicEngine();
//	//
//	//isSelecting = IS_SELECTING::NO;
//	//this->myType = myType;
//	//position_ = pos;
//	//color_ = VECTOR4(1, 1, 1, 1);
//	//
//	//switch (myType)
//	//{
//	//case MY_TYPE::GO_SELECT:
//	//	spr_ = graphicEngine->createSprite(L"./Data/Asset/Images/Sprites/Start.png");
//	//	size_ = 1;
//	//	scale_ = VECTOR2(1, 1);
//	//	texturePosition_ = VECTOR2(0, 0);
//	//	textureSize_ = VECTOR2(482, 200);
//	//	hSize_ = textureSize_;
//	//	break;
//	//case MY_TYPE::EXIT_GAME:
//	//	spr_ = graphicEngine->createSprite(L"./Data/Asset/Images/Sprites/End.png");
//	//	size_ = 1;
//	//	scale_ = VECTOR2(1, 1);
//	//	texturePosition_ = VECTOR2(0, 0);
//	//	textureSize_ = VECTOR2(482, 200);
//	//	hSize_ = textureSize_;
//	//	break;
//	//case MY_TYPE::BACK_GAME:
//	//	spr_ = graphicEngine->createSprite(L"./Data/Asset/Images/Sprites/Continue.png");
//	//	size_ = 1;
//	//	scale_ = VECTOR2(1, 1);
//	//	texturePosition_ = VECTOR2(0, 0);
//	//	textureSize_ = VECTOR2(635, 115);
//	//	hSize_ = textureSize_;
//	//	break;
//	//case MY_TYPE::RETRY:
//	//	spr_ = graphicEngine->createSprite(L"./Data/Asset/Images/Sprites/Retry.png");
//	//	size_ = 1;
//	//	scale_ = VECTOR2(1, 1);
//	//	texturePosition_ = VECTOR2(0, 0);
//	//	textureSize_ = VECTOR2(505, 135);
//	//	hSize_ = textureSize_;
//	//	break;
//	//case MY_TYPE::GO_TITLE:
//	//	spr_ = graphicEngine->createSprite(L"./Data/Asset/Images/Sprites/BackToTitle.png");
//	//	size_ = 1;
//	//	scale_ = VECTOR2(1, 1);
//	//	texturePosition_ = VECTOR2(0, 0);
//	//	textureSize_ = VECTOR2(450, 145);
//	//	hSize_ = textureSize_;
//	//	break;
//	//default:
//	//	break;
//	//}
//}
//
//void Button2D::update(bool isMouse)
//{
//	if (isMouse)
//	{
//		//if (MousePoint::get()->onClickTrigger(*this))
//		//{
//		//	decision();
//		//}
//		//
//		//if (MousePoint::get()->onTriggerNoMoveEnble(*this))
//		//{
//		//	color_ = { 1, 1, 1, 1 };
//		//}
//		//else color_ = { 0.5f, 0.5f, 0.5f, 1 };
//	}
//	else
//	{
//		//if (MousePoint::get()->onTriggerNoMoveEnble(*this))
//		//{
//		//	Button2DManager::get()->setIsMouse(true);
//		//}
//	}
//}
//
//void Button2D::decision()
//{
//	//switch (myType)
//	//{
//	//case MY_TYPE::GO_SELECT:
//	//	SceneManagerOld::get()->changeScene(SCENESTAGE, 0);
//	//	break;
//	//case MY_TYPE::EXIT_GAME:
//	//	GraphicEngine::get()->setExitGame(true);
//	//	break;
//	//case MY_TYPE::BACK_GAME:
//	//	SceneManagerOld::get()->getSceneNow()->setPause(false);
//	//	//CameraManager::get()->setActiveControl(true);
//	//	break;
//	//case MY_TYPE::RETRY:
//	//	SceneManagerOld::get()->getSceneNow()->setgameReset(true);
//	//	break;
//	//case MY_TYPE::GO_TITLE:
//	//	SceneManagerOld::get()->changeScene(SCENETITLE, 0);
//	//	break;
//	//default:
//	//	break;
//	//}
//}