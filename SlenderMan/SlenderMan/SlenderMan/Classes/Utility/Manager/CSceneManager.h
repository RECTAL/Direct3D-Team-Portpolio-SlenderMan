#pragma once
#include "../../Define/KGlobalDefine.h"
#include "../Interface/IUpdateable.h"
#include "../Interface/IRenderable.h"
#include "../Interface/IWindowMessageHandler.h"

class CScene;
class CSceneManager:public IUpdateable,IRenderable,IWindowMessageHandler
{
public:
	typedef std::unordered_map<std::string, CScene*> SceneMapList;
public:
	virtual ~CSceneManager();
public:
	//singleton declare
	DECLARE_SINGLETON(CSceneManager);

public:				//interface
	virtual void update(void)override;
	virtual void draw(void)override;
	virtual void drawUI(void)override;

	virtual LRESULT handleWindowMessage(HWND a_hWindow,
		UINT a_nMessage,
		WPARAM a_wParam,
		LPARAM a_lParam) override;

public:
	//inialization
	void init();
	void release();

	CScene* changeScene_rootDirect(std::string a_stSceneName,bool isInit = true);
	CScene* changeScene_rootLoadingScene(std::string a_stNextSceneName,bool isInit = true);
	CScene* findScene(std::string a_stSceneName);
	HRESULT deleteScene(std::string a_stSceneName);

private:
	HRESULT addScene(std::string a_stSceneName, CScene* a_pScene);




private:
	SceneMapList m_oSceneMapList;
	CScene*		 m_pCurrentScene;

};