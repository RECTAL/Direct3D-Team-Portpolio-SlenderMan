#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

class CSpriteObject_Button;
class CSpriteObject_Container;
class CCameraObject;
class CTerrainObject;
class CStaticObject;
class CLightObject;
class CSpotLightObject;
class CStage;
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

private:
	void	createRenderTarget();
	void	createCamera();
	void	createSound();

public:
	CSpotLightObject* createSpotObj();
private:
	void createContainer();
	void createButton();
private:
	POINT m_stPrevMousePosition;

	CCameraObject* m_pCamera = nullptr;
	CSpotLightObject* m_pSpotObj = nullptr;

	LPD3DXMESH m_pSphere = nullptr;


	CStage* m_pStage;
	bool isFirst = true;

	CSpriteObject_Button* exitButton = nullptr;
	CSpriteObject_Container* menuContainer = nullptr;

	FPTR* crashFptr = nullptr;
	FPTR* beginFptr = nullptr;
	FPTR* pressFptr = nullptr;
	FPTR* endFptr = nullptr;
};