#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../Interface/IWindowMessageHandler.h"

//! windowApplication
class CWindowApplication : public IWindowMessageHandler
{
protected:		// constructor, destructor

	//! constructor
	CWindowApplication(HINSTANCE a_hInstance,
		const SIZE &a_rstWindowSize,
		int a_nShowOptions);

	//! destructor
	virtual ~CWindowApplication(void);


public:			// interface

	//! interface: IWindowMessageHandler
	virtual LRESULT handleWindowMessage(HWND a_hWindow,
		UINT a_nMessage,
		WPARAM a_wParam,
		LPARAM a_lParam) override;

public:			// getter, setter
	/*************** getter ******************/
	//! getInstance
	static CWindowApplication * getInstance(void);



	/*************** setter ******************/
	//! setWindowSize
	virtual void setWindowSize(const SIZE &a_rstWindowSize);

public:	

	//! runApplication
	int runApplication(void);

protected:

	//! initialization Application
	virtual void initApplication(void);

	//! destroyMessage
	virtual void handleDestroyMessage(WPARAM a_wParam, LPARAM a_lParam);

	//! run MessageLoop
	virtual int runMessageLoop(void);


protected:

	int m_nShowOptions = 0;

	SIZE m_stWindowSize;
	HINSTANCE m_hInstance = nullptr;
};
