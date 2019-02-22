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
	// UI 생성
	void createDefaultUI();
	void createButtonUI();
	void createContainerButtonUI();

	// 컨테이너 생성
	void createContainer();

	// 사운드 생성
	void createSound();

	// 이미지 업데이트
	void defaultImageUpdate();
	void buttonImageUpdate();

	// 이미지 그리기
	void defaultImageDrawUI();
	void buttonImageDrawUI();

	// 볼륨 조절
	void setVolume();

	// 제거
	void releaseUI();
	void releaseImg();
	
private:
	CSpriteObject_Default*		m_pSpriteBackGround = nullptr;
	CSpriteObject_Default*		m_pTitleImage = nullptr;
	CSpriteObject_Default*		m_pSpriteFire = nullptr;

	CSpriteObject_Button*		m_pPlayButton = nullptr;
	CSpriteObject_Button*		m_pOptionButton = nullptr;
	CSpriteObject_Button*		m_pMapToolButton = nullptr;
	CSpriteObject_Button*		m_pExitButton = nullptr;
	CSpriteObject_Button*		m_pBackButton = nullptr;
	CSpriteObject_Button*		m_pSoundScrollBarButton[2] = { nullptr };

	CSpriteObject_Container*	m_pOptionWindow = nullptr;
	CSpriteObject_ScrollBar*	m_pSoundScrollBar[2] = { nullptr };

	CSpriteObject*				m_pCurrentSpriteHandle = nullptr;

	FPTR* crashFptr = nullptr;
	FPTR* beginFptr = nullptr;
	FPTR* pressFptr = nullptr;
	FPTR* endFptr = nullptr;

	bool m_bIsFirst = true;
	bool m_bIsStartSound = true;
};