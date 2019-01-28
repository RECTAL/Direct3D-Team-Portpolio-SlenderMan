#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"


class CSpriteObject;
class CSpriteObject_Button;
class CSpriteObject_Container;
class CSpriteObject_List;

class CTitleScene :public CScene
{
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
	CSpriteObject*	m_pSprite_BackGround;
	CSpriteObject* titleImage;
	CSpriteObject* sprite_fire;
	CSpriteObject* gameStartImage;
	CSpriteObject* optionImage;
	CSpriteObject* exitImage;
	CSpriteObject_Button* gameStartImage;

	CSpriteObject_Container* uiContainer;
	CSpriteObject_Button* gameStartButton;

	CSpriteObject_List*		 uiList;
	CSpriteObject_Button*	uiButton[5];
};