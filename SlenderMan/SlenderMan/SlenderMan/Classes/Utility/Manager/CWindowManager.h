#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../Interface/IWindowMessageHandler.h"

//! WindowManager
class CWindowManager : public IWindowMessageHandler
{

private:			// constructer, destructor

	//! constructer
	CWindowManager(void);

	//! destructor
	virtual ~CWindowManager(void);


public:			// interface

	//! interface: IWindowMessageHandler
	virtual LRESULT handleWindowMessage(HWND a_hWindow,
		UINT a_nMessage,
		WPARAM a_wParam,
		LPARAM a_lParam) override;

public:			// getter,setter
	/***************** getter **********************/
	//! getWindowHandle
	HWND getWindowHandle(void) const;

	//! getWindowInstance
	HINSTANCE getInstanceHandle(void) const;

	//! getWindowSize
	SIZE getWindowSize(void) const;




public:	

	//! singleton Declare
	DECLARE_SINGLETON(CWindowManager);

	//! initialization
	void init(HINSTANCE a_hInstance,
		const SIZE &a_rstWindowSize,
		IWindowMessageHandler *a_pMessageHandler);

private:			

	//! createWindow
	HWND createWindow(HINSTANCE a_hInstance,
		const SIZE &a_rstWindowSize);
private:			

	SIZE m_stWindowSize;

	HWND m_hWindow = nullptr;
	HINSTANCE m_hInstance = nullptr;

	LPCTSTR m_pszClassName = nullptr;
	IWindowMessageHandler *m_pMessageHandler = nullptr;
};
