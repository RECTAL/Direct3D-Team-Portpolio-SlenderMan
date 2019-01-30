#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

class CStaticObject;

class CMainPlayScene :public CScene
{
public:		//constructor , destructor
	CMainPlayScene(std::string a_stSceneName);
	virtual ~CMainPlayScene();

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

public:


private:


};