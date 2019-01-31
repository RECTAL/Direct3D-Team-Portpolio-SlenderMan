#pragma once

#include "../../Define/KGlobalDefine.h"
#include "CWindowApplication.h"
#include "../Interface/IUpdateable.h"
#include "../Interface/IRenderable.h"


//! direct3D Application
class CDirect3DApplication : public CWindowApplication,
	public IUpdateable,
	public IRenderable
{

public:			// constructor, destructor

	//! constructor
	CDirect3DApplication(HINSTANCE a_hInstance,
		const SIZE &a_rstWindowSize,
		int a_nShowOptions);

	//! destructor
	virtual ~CDirect3DApplication(void);


public:			// Interface

	//! interface: IUpdateable
	virtual void update(void) override;

	//! interface: IRenderable
	virtual void draw(void) override;

	//! interface: IRenderable
	virtual void drawUI(void) override;

	//! interface: IWindowMessageHandler
	virtual LRESULT handleWindowMessage(HWND a_hWindow,
		UINT a_nMessage,
		WPARAM a_wParam,
		LPARAM a_lParam) override;

public:			// getter,setter


	/************** setter *****************/
	//! changeWindow Size
	virtual void setWindowSize(const SIZE &a_rstWindowSize) override;

public:			

	//! initialization Application
	virtual void initApplication(void) override;

	//! run MessageLoop
	virtual int runMessageLoop(void) final override;

private:			
	LPD3DXSPRITE   createSprite();

protected:			

	POINT		m_stPrevMousePosition;

private:
	LPD3DXSPRITE	m_pSprite = nullptr;
};
