#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

class ILoader;
class CSpriteObject_Default;

class CLoadingScene :public CScene
{
public:		//constructor , destructor
	CLoadingScene(std::string a_stSceneName);
	virtual ~CLoadingScene();

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

public:		//	getter,setter
	/**************************************************************/
	//getter
	/**************************************************************/
	std::string getNextSceneName() { return m_stNextSceneName; }
	/**************************************************************/
	//setter
	/**************************************************************/
	void setNextSceneName(std::string a_pSceneName) { m_stNextSceneName = a_pSceneName; }

private:	//private function
	void	loadResources();						//���ҽ� �ε� �Լ�		�ڽľ����� Ȱ��

private:	
	std::thread*		m_pLoadThread;
	std::mutex			m_stMutex;


	std::string			m_stNextSceneName;
	ILoader*			m_pLoader = nullptr;

	bool				m_bIsAllDownLoad = false;

private:	// private �Լ�

	//! ��׶��带 �����
	void createBackGround(void);

	void createRenderTarget(void);

	//! �̹����� ������Ʈ�Ѵ�
	void updateImage(void);

private:
	
	CSpriteObject_Default * m_pBackGround[3] = { nullptr };
	int m_nIndex = 0;
	float alpha = 0.0f;

	bool isFirst = true;
};