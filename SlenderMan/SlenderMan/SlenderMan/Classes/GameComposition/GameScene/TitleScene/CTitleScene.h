#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"


class CSpriteObject;
class CSpriteObject_Button;
class CSpriteObject_Container;
class CSpriteObject_List;
class CSpriteObject_Default;

class CTitleScene :public CScene
{
private:
	typedef std::function<void(void)> FPTR;

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

	void defaultImageUpdate();
	void buttonImageUpdate();

	void defaultImageDrawUI();
	void buttonImageDrawUI();

private:
	CSpriteObject_Default* m_pSprite_BackGround;
	CSpriteObject_Default* titleImage;
	CSpriteObject_Default* sprite_fire;

	CSpriteObject_Button* playImage;
	CSpriteObject_Button* optionImage;
	CSpriteObject_Button* mapToolImage;
	CSpriteObject_Button* exitImage;

	//CSpriteObject_Button* gameStartImage;

	CSpriteObject_Container* uiContainer;
	//CSpriteObject_Button* gameStartButton;

	CSpriteObject_List*		 uiList;
	CSpriteObject_Button*	uiButton[5];

	CSpriteObject*			 m_pCurrentSpriteHandle =nullptr;

	FPTR* crashFptr;
	FPTR* beginFptr;
	FPTR* pressFptr;
	FPTR* endFptr;

};