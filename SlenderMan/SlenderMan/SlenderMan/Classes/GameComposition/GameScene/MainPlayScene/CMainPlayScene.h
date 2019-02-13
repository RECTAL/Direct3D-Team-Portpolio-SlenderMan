#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

class CSpriteObject_Button;
class CSpriteObject_Default;
class CSpriteObject_Container;
class CCameraObject;
class CTerrainObject;
class CStaticObject;
class CLightObject;
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
	void	createCamera();
	void	createStageSound();
	void	setStateSound();
	void	setBGMSound();
	void	setTimer();

public:
	CSpotLightObject* createSpotObj();
private:
	void createContainer();
	void createButton();

	void createLabel();
	void createSpriteDefault();



	void calcPlayTime(float a_fTime,int& a_nHour,int& a_nMin,int& a_nSec);
private:
	POINT pt;
	DWORD mousePositionX = 0;
	DWORD mousePositionY = 0;

	CCameraObject* m_pCamera = nullptr;
	CSpotLightObject* m_pSpotObj = nullptr;

	LPD3DXMESH m_pSphere = nullptr;

	CStage* m_pStage;
	bool isFirst = true;
	bool isBGMPlay = true;

	float m_fPlayTime = 0.0f;

	CSpriteObject_Default* m_pCamCoderView = nullptr;
	CLabelObject*		   m_pPlayTime = nullptr;

	CSpriteObject_Button* exitButton = nullptr;
	CSpriteObject_Container* menuContainer = nullptr;

	EPlayerState m_ePlayerState = EPlayerState::NONE;
	EPlayingBGM m_ePlayingBGM = EPlayingBGM::NONE;
	EStageSound m_eStageSound = EStageSound::STAGE_1;

	FPTR* crashFptr = nullptr;
	FPTR* beginFptr = nullptr;
	FPTR* pressFptr = nullptr;
	FPTR* endFptr = nullptr;

	CSpotLightObject** ppSpotLightObj;
	CLightObject** ppPointLightObj;

	// 움직인 시간
	float m_fRunTime = 0.0f;
};