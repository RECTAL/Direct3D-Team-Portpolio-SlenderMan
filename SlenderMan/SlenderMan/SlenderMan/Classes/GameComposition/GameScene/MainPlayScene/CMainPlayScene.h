#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

class player;
class CSpriteObject_Button;
class CSpriteObject_Default;
class CSpriteObject_Container;
class CSpriteObject_ScrollBar;
class CCameraObject;
class CTerrainObject;
class CStaticObject;
class CLightObject;
class CSpriteObject;
class CSpotLightObject;
class CStage;
class CLabelObject;

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
	void	createStageSound();
	void	setStateSound();
	void	setBGMSound();
	void	setTimer();
	void	setVolume();
	void	setPlayState();

public:
	//getter,setter
	/*********************************************/
	//getter
	/*********************************************/
	bool getIsMenu() { return m_bIsMenu; }
public:
	CSpotLightObject* createSpotObj();

private:
	void createContainer();
	void createButton();
	void releaseUI();

	void createLabel();
	void createSpriteDefault();
	void calcPlayTime(float a_fTime,int& a_nHour,int& a_nMin,int& a_nSec);

	void settingPlayer();
private:
	LPD3DXMESH m_pSphere = nullptr;

	CStage* m_pStage;
	bool m_bIsFirst = true;
	bool m_bIsBGMPlay = true;
	bool m_bIsMenu = false;

	float m_fPlayTime = 0.0f;

	CSpriteObject_Default* m_pCamCoderView = nullptr;
	CLabelObject*		   m_pPlayTime = nullptr;
	CSpriteObject*		   m_pCurrentSpriteHandle = nullptr;

	CSpriteObject_Button* m_pExitButton = nullptr;
	CSpriteObject_Button* m_pOptionButton = nullptr;
	CSpriteObject_Button* m_pBackButton = nullptr;
	CSpriteObject_Button* m_pScrollBarButton[2] = { nullptr };
	
	CSpriteObject_Container* m_pMenuContainer = nullptr;
	CSpriteObject_Container* m_pSoundContainer = nullptr;
	
	CSpriteObject_ScrollBar* m_pScrollBar[2] = { nullptr };

	EPlayingBGM m_ePlayingBGM = EPlayingBGM::NONE;
	EStageSound m_eStageSound = EStageSound::STAGE_1;

	FPTR* crashFptr = nullptr;
	FPTR* beginFptr = nullptr;
	FPTR* pressFptr = nullptr;
	FPTR* endFptr = nullptr;

	CSpotLightObject** ppSpotLightObj;
	CLightObject** ppPointLightObj;

	player* pPlayer = nullptr;

	// 움직인 시간
	float m_fRunTime = 0.0f;
};