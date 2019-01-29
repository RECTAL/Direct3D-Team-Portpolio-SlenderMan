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
	SAFE_DELETE(sprite_fire);
	SAFE_DELETE(titleImage);
	SAFE_DELETE(playImage);
	SAFE_DELETE(optionImage);
	SAFE_DELETE(exitImage);
	
	SAFE_DELETE(uiContainer);

	SAFE_DELETE(uiContainer);
	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(uiButton[i]);
	}
}

void CTitleScene::init()
{
	CScene::init();

	this->createDefaultUI();
	this->createWindowUI();
}

void CTitleScene::createWindowUI()
{
	/********************************************************/
	//버튼 UI
	this->createButtonUI();
	//UI 창 컨테이너
	/********************************************************/
	uiContainer = new CSpriteObject_Container("Resources/Textures/Scene/TitleScene/ExWindow", "png",600,619, 1);
	uiContainer->setPosition(D3DXVECTOR3(600, 500, 0));
	uiContainer->setVisible(false);
	uiContainer->init(nullptr, nullptr, nullptr, nullptr);
	/**********************************************/
	// 창 컨테이너 안의 버튼 추가
	/**********************************************/
	/*gameStartButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/gameStart", "png", 1);
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
		D3DXVECTOR3(20.0f, 30.0f, 0.0f)
	);*/

	//uiContainer->addChildSpriteObject("gameStartButton", CWindowType::BUTTON, gameStartButton);
	/********************************************************/
	//게임 시작 버튼
	/********************************************************/
	/*gameStartImage = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/gameStart", "png", 1);
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
	);*/

	/********************************************************/
	//ui 리스트
	/********************************************************/
	uiList = new CSpriteObject_List("Resources/Textures/Scene/TitleScene/ExWindow", "png", 600,619,1);
	uiList->setPosition(D3DXVECTOR3(1300, 500, 0));
	uiList->setVisible(false);
	uiList->init(nullptr, nullptr, nullptr, nullptr);
	/**********************************************/
	//리스트 안의 내용 삽입
	/**********************************************/
	for (int i = 0; i < 5; i++)
	{
		uiButton[i] = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/gameStart", "png", 200,80,1);
		uiButton[i]->setPosition(uiList->getPosition());
		std::function<void(void)>* fptr1 = new std::function<void(void)>;
		(*fptr1) = [=](void)->void
		{
			CHANGE_SCENE_LOADING(GAMESCENE_MAINPLAY);
		};
		uiButton[i]->init(
			nullptr,
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
	 
void CTitleScene::createDefaultUI()
{
	m_pSprite_BackGround = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/background", "png",1366,768, 1);
	m_pSprite_BackGround->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));

	sprite_fire = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/fire/fire", "png", 1366 ,768 ,33);
	sprite_fire->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));

	titleImage = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/title", "png", 760 , 462, 1);
	titleImage->setPosition(D3DXVECTOR3(450, 250, 0));
}

void CTitleScene::createButtonUI()
{
	playImage = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/play", "png",425,129, 1);
	playImage->setPosition(D3DXVECTOR3(400, 550, 0));
	std::function<void(void)>* fptr = new std::function<void(void)>;
	(*fptr) = [=](void) -> void
	{
		CHANGE_SCENE_LOADING(GAMESCENE_MAINPLAY);
	};
	playImage->init(nullptr, nullptr, nullptr, fptr);

	optionImage = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/option", "png",634,127, 1);
	optionImage->setPosition(D3DXVECTOR3(400, 750, 0));
	
	(*fptr) = [=](void) -> void
	{
		uiContainer->setVisible(!uiContainer->getVisible());
	};
	optionImage->init(nullptr, nullptr, nullptr, fptr);

	(*fptr) = [=](void) -> void
	{
		SendMessage(GET_WINDOW_HANDLE(), WM_DESTROY, 0, 0);
	};

	exitImage = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/exit", "png", 395,130,1);
	exitImage->setPosition(D3DXVECTOR3(400, 950, 0));
	exitImage->init(nullptr, nullptr, nullptr, fptr);

}

void CTitleScene::update(void)
{
	CScene::update();
	this->defaultImageUpdate();
	this->buttonImageUpdate();

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

void CTitleScene::defaultImageUpdate()
{
	m_pSprite_BackGround->update();

	static float time = 0.0f;
	time += GET_DELTA_TIME();
	if (time >= 0.03f)
	{
		sprite_fire->update();
		time = 0;
	}
	titleImage->update();
}

void CTitleScene::buttonImageUpdate()
{
	playImage->update();
	optionImage->update();
	exitImage->update();
}

void CTitleScene::draw(void)
{
	CScene::draw();
}

void CTitleScene::drawUI(void)
{
	CScene::drawUI();
	this->defaultImageDrawUI();
	this->buttonImageDrawUI();

	uiContainer->drawUI();
	uiList->drawUI();
}

void CTitleScene::defaultImageDrawUI()
{
	m_pSprite_BackGround->drawUI();
	sprite_fire->doDrawUI();
	titleImage->drawUI();
}

void CTitleScene::buttonImageDrawUI()
{
	optionImage->drawUI();
	exitImage->drawUI();
	playImage->drawUI();
}

LRESULT CTitleScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}
