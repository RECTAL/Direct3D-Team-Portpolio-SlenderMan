#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"


class CSpriteObject;
class CSpriteObject_Button;
class CSpriteObject_Container;
class CSpriteObject_List;
class CSpriteObject_Default;
class CSpriteObject_ScrollBar;
class CLoadingScene;


class CTitleScene :public CScene
{
private:
	

public:		//constructor , destructor
	CTitleScene(std::string a_stSceneName);
	virtual ~CTitleScene();

public:		//interface
	virtual void update(void)override;				//interface : IUpdateable
	virtual void draw(void)override;				//interface : IRenderable
	virtual void drawUI(void)override;				//interface : IRenderable

	virtual LRESULT handleWindowMessage(HWND a_hWindow,
		UINT a_nMessage,
		WPARAM a_wParam,
		LPARAM a_lParam) override;					//interface : IWindowMessageHandler

public:		//public override function
	virtual void init()override;					//override	: CScene
	virtual void createWindowUI()override;

private:
	void createDefaultUI();
	void createButtonUI();
	void createSound();

	void defaultImageUpdate();
	void buttonImageUpdate();

	void defaultImageDrawUI();
	void buttonImageDrawUI();

	void setVolume();
	
private:
	CSpriteObject_Default* m_pSprite_BackGround = nullptr;
	CSpriteObject_Default* titleImage = nullptr;
	CSpriteObject_Default* sprite_fire = nullptr;

	CSpriteObject_Button* playButton = nullptr;
	CSpriteObject_Button* optionButton = nullptr;
	CSpriteObject_Button* mapToolButton = nullptr;
	CSpriteObject_Button* exitButton = nullptr;


	CSpriteObject_Container* optionWindow = nullptr;
	CSpriteObject_Button* backButton = nullptr;

	CSpriteObject_ScrollBar* soundScrollBar[2] = { nullptr };
	CSpriteObject_Button*	 soundScrollBarButton[2] = { nullptr };

	CSpriteObject*			 m_pCurrentSpriteHandle = nullptr;

	FPTR* crashFptr = nullptr;
	FPTR* beginFptr = nullptr;
	FPTR* pressFptr = nullptr;
	FPTR* endFptr = nullptr;

	bool isFirst = true;
	bool isStartSound = true;
};