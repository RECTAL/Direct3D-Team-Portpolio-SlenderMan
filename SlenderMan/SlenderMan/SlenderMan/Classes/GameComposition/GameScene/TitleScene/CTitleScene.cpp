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

	SAFE_DELETE(m_pSprite_BackGround);
	SAFE_DELETE(sprite_fire);
	SAFE_DELETE(titleImage);
	SAFE_DELETE(playButton);
	SAFE_DELETE(optionButton);
	SAFE_DELETE(mapToolButton);
	SAFE_DELETE(exitButton);
	SAFE_DELETE(backButton);
	
	SAFE_DELETE(optionWindow);

	//SAFE_DELETE(soundScrollBar);

	/*for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE(uiButton[i]);
	}*/
}

void CTitleScene::init()
{
	CScene::init();
	if (isFirst)
	{
		crashFptr = new std::function<void(void)>;
		beginFptr = new std::function<void(void)>;
		pressFptr = new std::function<void(void)>;
		endFptr = new std::function<void(void)>;

		this->createDefaultUI();
		this->createWindowUI();
		isFirst = false;
	}
	GET_SOUND_MANAGER()->stopAllEffectSounds();
	isStartSound = true;
}

void CTitleScene::createWindowUI()
{
	/********************************************************/
	//��ư UI
	/********************************************************/
	this->createButtonUI();

	/********************************************************/
	//UI â �����̳�
	/********************************************************/
	optionWindow = new CSpriteObject_Container("Resources/Textures/Scene/TitleScene/optionWindow", "png", 800, 640, 1);
	optionWindow->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));
	optionWindow->setVisible(false);
	optionWindow->init(nullptr, nullptr, nullptr, nullptr);

	/**********************************************/
	// â �����̳� ���� ��ư �߰�
	/**********************************************/
	backButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/back", "png", 200, 100, 2);
	(*crashFptr) = [=](void)->void
	{
		backButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void)->void
	{
		m_pCurrentSpriteHandle = nullptr;
		soundScrollBar->setVisible(false);
		optionWindow->setVisible(false);
	};
	backButton->init(
		crashFptr,
		nullptr,
		nullptr,
		endFptr,
		true,
		D3DXVECTOR3(260.0f, 230.0f, 0.0f)
	);

	optionWindow->addChildSpriteObject("backButton", CWindowType::BUTTON, backButton);
	optionWindow->addChildSpriteObject("soundScroll", CWindowType::SCROLLBAR, soundScrollBar);

	/********************************************************/
	//ui ����Ʈ
	/********************************************************/
	/*uiList = new CSpriteObject_List("Resources/Textures/Scene/TitleScene/ExWindow", "png", 300, 310, 1);
	uiList->setPosition(D3DXVECTOR3(1300, 500, 0));
	uiList->setVisible(false);
	uiList->init(nullptr, nullptr, nullptr, nullptr);*/
	/**********************************************/
	//����Ʈ ���� ���� ����
	/**********************************************/
	/*for (int i = 0; i < 5; i++)
	{
		uiButton[i] = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/gameStart", "png", 200,80,1);
		uiButton[i]->setPosition(uiList->getPosition());
		
		(*endFptr) = [=](void)->void
		{
			CHANGE_SCENE_LOADING(GAMESCENE_MAINPLAY,TRUE);
		};
		uiButton[i]->init(
			nullptr,
			nullptr,
			nullptr,
			endFptr,
			true,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		);
		char name[100];
		sprintf(name, "uiButton_%d", i);
		uiList->addChildSpriteObject(name, CWindowType::BUTTON, uiButton[i]);
	}*/
}
	 
void CTitleScene::createDefaultUI()
{
	m_pSprite_BackGround = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/background", "png", 1366, 768, 1, false);
	m_pSprite_BackGround->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));

	sprite_fire = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/fire/fire", "png", 1366, 768, 33, false);
	sprite_fire->setPosition(D3DXVECTOR3(GET_WINDOW_SIZE().cx / 2, GET_WINDOW_SIZE().cy / 2, 0));

	titleImage = new CSpriteObject_Default("Resources/Textures/Scene/TitleScene/title", "png", 600, 300, 1, false);
	titleImage->setPosition(D3DXVECTOR3(380, 200, 0));
}

void CTitleScene::createButtonUI()
{
	playButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/play", "png", 300, 60, 2);
	playButton->setPosition(D3DXVECTOR3(370, 390, 0));
	(*crashFptr) = [=](void) -> void
	{
		playButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Click.wav", false);
		CHANGE_SCENE_LOADING(GAMESCENE_MAINPLAY,TRUE);
	};
	playButton->init(crashFptr, nullptr, nullptr, endFptr);

	soundScrollBar = new CSpriteObject_ScrollBar("Resources/Textures/Scene/TitleScene/whiteCover", "png", 300, 20, 1);

	soundScrollBarButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/grayCover", "png", 10, 50, 1);
	soundScrollBarButton->init(nullptr, nullptr, nullptr, nullptr, true);

	soundScrollBar->init(nullptr, nullptr, nullptr, nullptr, 0, 300, soundScrollBarButton, true, D3DXVECTOR3(0, -180, 0.0f));

	optionButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/option", "png", 350, 60, 2);
	optionButton->setPosition(D3DXVECTOR3(370, 490, 0));
	(*crashFptr) = [=](void) -> void
	{
		optionButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Click.wav", false);
		m_pCurrentSpriteHandle = optionWindow;
		optionWindow->setVisible(true);
		soundScrollBar->setVisible(true);
	};
	optionButton->init(crashFptr, nullptr, nullptr, endFptr);

	mapToolButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/mapTool", "png", 450, 60, 2);
	mapToolButton->setPosition(D3DXVECTOR3(370, 590, 0));
	(*crashFptr) = [=](void) -> void
	{
		mapToolButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Click.wav", false);
		CHANGE_SCENE_LOADING(GAMESCENE_MAPTOOL,TRUE);
	};

	mapToolButton->init(crashFptr, nullptr, nullptr, endFptr);

	exitButton = new CSpriteObject_Button("Resources/Textures/Scene/TitleScene/exit", "png", 300, 60, 2);
	exitButton->setPosition(D3DXVECTOR3(370, 690, 0));

	(*crashFptr) = [=](void) -> void
	{
		exitButton->getTextureOffset() = 1;
	};

	(*endFptr) = [=](void) -> void
	{
		GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Click.wav", false);
		SendMessage(GET_WINDOW_HANDLE(), WM_DESTROY, 0, 0);
	};

	exitButton->init(crashFptr, nullptr, nullptr, endFptr);
}

void CTitleScene::createSound()
{
	GET_SOUND_MANAGER()->playBackgroundSound("Resources/Sounds/BGMSounds/Title.wav", true);
	GET_SOUND_MANAGER()->playEffectSound("Resources/Sounds/EffectSounds/Fire.wav", true);
}

void CTitleScene::update(void)
{
	CScene::update();
	this->defaultImageUpdate();
	if (m_pCurrentSpriteHandle == nullptr)
	{
		this->buttonImageUpdate();
		optionWindow->update();
	}
	else
	{
		m_pCurrentSpriteHandle->update();
	}
	printf("%f\n", soundScrollBar->getSetValue());

	if (isStartSound)
	{
		createSound();
		isStartSound = false;
	}
	this->setVolume();
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
	playButton->update();
	optionButton->update();
	mapToolButton->update();
	exitButton->update();
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

	optionWindow->drawUI();

	
	//uiList->drawUI();
}

void CTitleScene::defaultImageDrawUI()
{
	m_pSprite_BackGround->drawUI();
	sprite_fire->doDrawUI();
	titleImage->drawUI();
}

void CTitleScene::buttonImageDrawUI()
{
	playButton->drawUI();
	optionButton->drawUI();
	mapToolButton->drawUI();
	exitButton->drawUI();
}

void CTitleScene::setVolume()
{
	if (m_pCurrentSpriteHandle == optionWindow) {
		GET_SOUND_MANAGER()->setBackgroundSoundVolume(soundScrollBar->getSetValue() / 300);
	}
}


LRESULT CTitleScene::handleWindowMessage(HWND a_hWindow, UINT a_nMessage, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_nMessage)
	{

	}
	return S_OK;
}