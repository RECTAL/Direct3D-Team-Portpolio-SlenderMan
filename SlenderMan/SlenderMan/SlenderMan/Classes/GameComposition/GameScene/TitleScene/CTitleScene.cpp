#include "CTitleScene.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Button.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Container.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_List.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Manager/CInputManager.h"
CTitleScene::CTitleScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CTitleScene::~CTitleScene()
{
	SAFE_DELETE(m_pSprite_BackGround);
	SAFE_DELETE(titleImage);
	SAFE_DELETE(gameStartImage);
	SAFE_DELETE(uiContainer);
	SAFE_DELETE(gameStartButton);

	SAFE_DELETE(uiContainer);
	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(uiButton[i]);
	}
}

void CTitleScene::init()
{
	CScene::init();
	m_pSprite_BackGround = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/background", "png", 1);
	m_pSprite_BackGround->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
	
	titleImage = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/title", "png", 1);
	titleImage->setPosition(D3DXVECTOR3(450, 250, 0));

	this->createWindowUI();
}

void CTitleScene::createWindowUI()
{
	/********************************************************/
	//UI 창 컨테이너
	/********************************************************/
	uiContainer = new CSpriteObject_Container("Resources/Textures/Scene/TitleScene/ExWindow", "png", 1);
	uiContainer->setPosition(D3DXVECTOR3(600, 500, 0));
	uiContainer->init(nullptr,nullptr, nullptr);
		/**********************************************/
		// 창 컨테이너 안의 버튼 추가
		/**********************************************/
		gameStartButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/gameStart", "png", 1);
		std::function<void(void)>* fptr0 = new std::function<void(void)>;
		(*fptr0) = [=](void)->void
		{
			CHANGE_SCENE_LOADING(GAMESCENE_MAINPLAY);
		};
		gameStartButton->init(
			nullptr,
			nullptr,
			fptr0,
			true,
			D3DXVECTOR3(20.0f,30.0f,0.0f)
		);
	
	uiContainer->addChildSpriteObject("gameStartButton", CWindowType::BUTTON, gameStartButton);
	/********************************************************/
	//게임 시작 버튼
	/********************************************************/
	gameStartImage = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/gameStart", "png", 1);
	gameStartImage->setPosition(D3DXVECTOR3(300, 500, 0));

	std::function<void(void)>* fptr = new std::function<void(void)>;
	(*fptr) = [=](void)->void
	{
		CHANGE_SCENE_LOADING(GAMESCENE_MAINPLAY);
	};
	gameStartImage->init(
		nullptr,
		nullptr,
		fptr
	);


	/********************************************************/
	//ui 리스트
	/********************************************************/
	uiList = new CSpriteObject_List("Resources/Textures/Scene/TitleScene/ExWindow", "png", 1);
	uiList->setPosition(D3DXVECTOR3(1300, 500, 0));
	uiList->init(nullptr,nullptr,nullptr);
			/**********************************************/
			//리스트 안의 내용 삽입
			/**********************************************/
			for (int i = 0; i < 5; i++)
			{
				uiButton[i] = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/gameStart", "png", 1);
				uiButton[i]->setPosition(uiList->getPosition());
				std::function<void(void)>* fptr1 = new std::function<void(void)>;
				(*fptr1) = [=](void)->void
				{
					CHANGE_SCENE_LOADING(GAMESCENE_MAINPLAY);
				};
				uiButton[i]->init(
					nullptr,
					nullptr,
					fptr1,
					true,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f)
				);
				char name[100];
				sprintf(name, "uiButton_%d", i);
				uiList->addChildSpriteObject(name, CWindowType::BUTTON, uiButton[i]);
			}
}


void CTitleScene::update(void)
{
	CScene::update();
	m_pSprite_BackGround->update();
	static int alpha = 100.0f;
	static float angle = 0.0f;
	angle += 90 * GET_DELTA_TIME();
	alpha = 255 * cosf(D3DXToRadian(angle))+100;
	alpha = min(alpha, 255);
	alpha = max(alpha, 100);
	m_pSprite_BackGround->setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	titleImage->update();
	gameStartImage->update();
	uiContainer->update();
	uiList->update();

	//Sprite Container 안에 있는 윈도우들의 팝업창 예시
	if (IS_KEY_PRESSED(DIK_ESCAPE))
	{
		uiContainer->setVisible(!uiContainer->getVisible());
	}

	//Sprite List 안에 있는 윈도우들의 팝업창 예시
	if (IS_KEY_PRESSED(DIK_F1))
	{
		uiList->setVisible(!uiList->getVisible());
	}

	//Sprite List 안의 내용물 위치 변경
	static float moveValue = 0.0f;
	if (IS_KEY_DOWN(DIK_UP))
	{
		moveValue -= 150 * GET_DELTA_TIME();
	}
	if (IS_KEY_DOWN(DIK_DOWN))
	{
		moveValue += 150 * GET_DELTA_TIME();
	}
	uiList->getMoveOffset() = D3DXVECTOR3(0.0f, moveValue, 0.0f);
}

void CTitleScene::draw(void)
{
	CScene::draw();
}

void CTitleScene::drawUI(void)
{
	CScene::drawUI();
	m_pSprite_BackGround->drawUI();
	titleImage->drawUI();
	gameStartImage->drawUI();
	uiContainer->drawUI();
	uiList->drawUI();
}

LRESULT CTitleScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
