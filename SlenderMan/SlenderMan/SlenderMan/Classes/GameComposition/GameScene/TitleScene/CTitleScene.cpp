#include "CTitleScene.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Button.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Container.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Default.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_List.h"
#include "../../../Utility/Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_ScrollBar.h"
#include "../../../Utility/Manager/CWindowManager.h"
#include "../../../Utility/Manager/CTimeManager.h"
#include "../../../Utility/Manager/CSceneManager.h"
#include "../../../Utility/Manager/CInputManager.h"
#include "../../../Utility/Manager/CRendertargetManager.h"
#include "../../../Utility/Manager/CDeviceManager.h"
#include "../../../Utility/Manager/CSoundManager.h"
#include "../../../Function/GlobalFunction.h"
#include "../LoadingScene/CLoadingScene.h"

CTitleScene::CTitleScene(std::string a_stSceneName)
	:CScene(a_stSceneName)
{
}

CTitleScene::~CTitleScene()
{
	SAFE_DELETE(crashFptr);
	SAFE_DELETE(beginFptr);
	SAFE_DELETE(pressFptr);
	SAFE_DELETE(endFptr);
	
	this->releaseUI();
	this->releaseImg();
}

void CTitleScene::init()
{
	CScene::init();
	if (m_bIsFirst)
	{
		crashFptr = new std::function<void(void)>;
		beginFptr = new std::function<void(void)>;
		pressFptr = new std::function<void(void)>;
		endFptr = new std::function<void(void)>;

		this->createDefaultUI();
		this->createWindowUI();
		m_bIsFirst = false;
	}
	GET_SOUND_MANAGER()->stopAllEffectSounds();
	m_bIsStartSound = true;
	m_bIsShowCursor = TRUE;
}

void CTitleScene::update(void)
{
	CScene::update();

	ShowCursor(m_bIsShowCursor);
	GET_DEVICE()->ShowCursor(m_bIsShowCursor);
	this->defaultImageUpdate();
	if (m_pCurrentSpriteHandle == nullptr){
		this->buttonImageUpdate();
		m_pOptionWindow->update();
	}
	else{
		m_pCurrentSpriteHandle->update();
	}
	if (m_bIsStartSound){
		this->createSound();
		m_bIsStartSound = false;
	}
	this->setVolume();
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

	m_pOptionWindow->drawUI();
}


void CTitleScene::createWindowUI()
{
	/********************************************************/
	//타이틀 버튼 UI
	/********************************************************/
	this->createButtonUI();
	/********************************************************/
	//옵션 창 컨테이너 UI
	/********************************************************/
	this->createContainer();
	this->createContainerButtonUI();
}
	 
void CTitleScene::createDefaultUI()
{
	m_pSpriteBackGround = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/background", "png", 1366, 768, 1, false);
	m_pSpriteBackGround->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));

	m_pSpriteFire = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/fire/fire", "png", 1366, 768, 33, false);
	m_pSpriteFire->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));

	m_pTitleImage = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/title", "png", 600, 300, 1, false);
	m_pTitleImage->setPosition(D3DXVECTOR3(380, 200, 0));
}

void CTitleScene::createButtonUI()
{
	m_pPlayButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/play", "png", 300, 60, 2);
	m_pPlayButton->setPosition(D3DXVECTOR3(370, 390, 0));
	(*crashFptr) = [=](void) -> void
	{
		m_pPlayButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Click.wav", false);
		CHANGE_SCENE_LOADING(GAMESCENE_MAINPLAY,TRUE);
	};
	m_pPlayButton->init(crashFptr, nullptr, nullptr, endFptr);

	m_pOptionButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/option", "png", 350, 60, 2);
	m_pOptionButton->setPosition(D3DXVECTOR3(370, 490, 0));
	(*crashFptr) = [=](void) -> void
	{
		m_pOptionButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Click.wav", false);
		m_pCurrentSpriteHandle = m_pOptionWindow;
		m_pOptionWindow->setVisible(true);
		m_pSoundScrollBar[0]->setVisible(true);
		m_pSoundScrollBar[1]->setVisible(true);
	};
	m_pOptionButton->init(crashFptr, nullptr, nullptr, endFptr);

	m_pMapToolButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/mapTool", "png", 450, 60, 2);
	m_pMapToolButton->setPosition(D3DXVECTOR3(370, 590, 0));
	(*crashFptr) = [=](void) -> void
	{
		m_pMapToolButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Click.wav", false);
		CHANGE_SCENE_LOADING(GAMESCENE_MAPTOOL,TRUE);
	};

	m_pMapToolButton->init(crashFptr, nullptr, nullptr, endFptr);

	m_pExitButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/exit", "png", 300, 60, 2);
	m_pExitButton->setPosition(D3DXVECTOR3(370, 690, 0));

	(*crashFptr) = [=](void) -> void
	{
		m_pExitButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Click.wav", false);
		SendMessage(GET_WINDOW_HANDLE(), WM_DESTROY, 0, 0);
	};

	m_pExitButton->init(crashFptr, nullptr, nullptr, endFptr);
}

void CTitleScene::createContainer()
{
	m_pOptionWindow = new CSpriteObject_Container("Resources/Textures/Scene/TitleScene/optionWindow", "png", 800, 640, 1);
	m_pOptionWindow->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
	m_pOptionWindow->setVisible(false);
	m_pOptionWindow->init(nullptr, nullptr, nullptr, nullptr);
}

void CTitleScene::createContainerButtonUI()
{
	/**********************************************/
	// 옵션 창 컨테이너 안의 버튼 추가
	/**********************************************/
	m_pSoundScrollBar[0] = new CSpriteObject_ScrollBar("Resources/Textures/Scene/TitleScene/whiteCover", "png", 300, 20, 1);

	m_pSoundScrollBarButton[0] = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/grayCover", "png", 10, 50, 1);
	m_pSoundScrollBarButton[0]->init(nullptr, nullptr, nullptr, nullptr, true);

	m_pSoundScrollBar[0]->init(nullptr, nullptr, nullptr, nullptr, 0, 300, m_pSoundScrollBarButton[0], true, D3DXVECTOR3(30, -70, 0.0f));

	m_pSoundScrollBar[1] = new CSpriteObject_ScrollBar("Resources/Textures/Scene/TitleScene/whiteCover", "png", 300, 20, 1);

	m_pSoundScrollBarButton[1] = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/grayCover", "png", 10, 50, 1);
	m_pSoundScrollBarButton[1]->init(nullptr, nullptr, nullptr, nullptr, true);

	m_pSoundScrollBar[1]->init(nullptr, nullptr, nullptr, nullptr, 0, 300, m_pSoundScrollBarButton[1], true, D3DXVECTOR3(30, 10, 0.0f));

	m_pBackButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/back", "png", 200, 100, 2);
	(*crashFptr) = [=](void)->void
	{
		m_pBackButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void)->void
	{
		m_pCurrentSpriteHandle = nullptr;
		m_pSoundScrollBar[0]->setVisible(false);
		m_pSoundScrollBar[1]->setVisible(false);
		m_pOptionWindow->setVisible(false);
	};
	m_pBackButton->init(crashFptr, nullptr, nullptr, endFptr, true, D3DXVECTOR3(260.0f, 230.0f, 0.0f));

	m_pOptionWindow->addChildSpriteObject("backButton", CWindowType::BUTTON, m_pBackButton);
	m_pOptionWindow->addChildSpriteObject("bgmSoundScroll", CWindowType::SCROLLBAR, m_pSoundScrollBar[0]);
	m_pOptionWindow->addChildSpriteObject("effectSoundScroll", CWindowType::SCROLLBAR, m_pSoundScrollBar[1]);
}

void CTitleScene::createSound()
{
	GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/Title.wav", true);
	GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Fire.wav", true);
}

void CTitleScene::defaultImageUpdate()
{
	m_pSpriteBackGround->update();

	static float time = 0.0f;
	time += GET_DELTA_TIME();
	if (time >= 0.03f)
	{
		m_pSpriteFire->update();
		time = 0;
	}
	m_pTitleImage->update();
}

void CTitleScene::buttonImageUpdate()
{
	m_pPlayButton->update();
	m_pOptionButton->update();
	m_pMapToolButton->update();
	m_pExitButton->update();
}

void CTitleScene::defaultImageDrawUI()
{
	m_pSpriteBackGround->drawUI();
	m_pSpriteFire->doDrawUI();
	m_pTitleImage->drawUI();
}

void CTitleScene::buttonImageDrawUI()
{
	m_pPlayButton->drawUI();
	m_pOptionButton->drawUI();
	m_pMapToolButton->drawUI();
	m_pExitButton->drawUI();
}

void CTitleScene::setVolume()
{
	if (m_pCurrentSpriteHandle == m_pOptionWindow) {
		GET_SOUND_MANAGER()->setBackgroundSoundVolume(m_pSoundScrollBar[0]->getSetValue() / 100);
		GET_SOUND_MANAGER()->setEffectSoundsVolume(m_pSoundScrollBar[1]->getSetValue() / 100);
	}
}

void CTitleScene::releaseUI()
{
	// 버튼 제거
	SAFE_DELETE(m_pPlayButton);
	SAFE_DELETE(m_pOptionButton);
	SAFE_DELETE(m_pMapToolButton);
	SAFE_DELETE(m_pExitButton);
	SAFE_DELETE(m_pBackButton);
	for (int i = 0; i < 2; i++) {
		SAFE_DELETE(m_pSoundScrollBar[i]);
		SAFE_DELETE(m_pSoundScrollBarButton[i]);
	}

	// 컨테이너 제거
	SAFE_DELETE(m_pOptionWindow);
}

void CTitleScene::releaseImg()
{
	SAFE_DELETE(m_pSpriteBackGround);
	SAFE_DELETE(m_pSpriteFire);
	SAFE_DELETE(m_pTitleImage);
}

LRESULT CTitleScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}