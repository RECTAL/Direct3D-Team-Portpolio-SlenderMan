#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"


class CSpriteObject;
class CSpriteObject_Button;
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


private:
	CSpriteObject*	m_pSprite_BackGround;
	CSpriteObject* titleImage;
	CSpriteObject_Button* gameStartImage;
};