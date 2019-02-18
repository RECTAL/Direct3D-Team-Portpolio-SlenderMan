#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

class CSpriteObject_Default;
class CGameOverScene :public CScene
{
public:		//constructor , destructor
	CGameOverScene(std::string a_stSceneName);
	virtual ~CGameOverScene();

public:		//interface
	virtual void update(void) override;				//interface : IUpdateable
	virtual void draw(void) override;				//interface : IRenderable
	virtual void drawUI(void) override;				//interface : IRenderable

	virtual LRESULT handleWindowMessage(HWND a_hWindow,
		UINT a_nMessage,
		WPARAM a_wParam,
		LPARAM a_lParam) override;					//interface : IWindowMessageHandler

public:		//public override function
	virtual void init() override;					//override	: CScene
	virtual void createWindowUI() override;

private:	//private 함수
	void createScene(void);
	void updateSprite(void);
	void createSound(void);

private:	//private 변수
	CSpriteObject_Default * m_pGameOverSprite = nullptr;
	CSpriteObject_Default * m_pDeadSprite = nullptr;
	int m_nCount = 0;
	float m_fCurrentTime = 0.0f;

	bool isFirst = true;
	bool m_bIsStartSound;
};