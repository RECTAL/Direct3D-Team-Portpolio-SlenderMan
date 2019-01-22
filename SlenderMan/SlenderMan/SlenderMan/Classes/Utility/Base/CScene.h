#pragma once
#include "../../Define/KGlobalDefine.h"
#include "../Interface/IUpdateable.h"
#include "../Interface/IRenderable.h"
#include "../Interface/IWindowMessageHandler.h"

class CScene :public IUpdateable,IRenderable,IWindowMessageHandler
{
public:
	CScene(std::string a_stSceneName);
	virtual ~CScene();

public:
	//inialization
	virtual void init();


public:		//interface
	virtual void update(void)override;				//interface : IUpdateable
	virtual void draw(void)override;				//interface : IRenderable
	virtual void drawUI(void)override;

	virtual LRESULT handleWindowMessage(HWND a_hWindow,
		UINT a_nMessage,
		WPARAM a_wParam,
		LPARAM a_lParam) override;					//interface : IWindowMessageHandler

public:		//getter,setter
	/**********************************************/
	//getter
	/**********************************************/
	std::string getSceneName() { return m_stSceneName; }





protected:
	std::string m_stSceneName;

};