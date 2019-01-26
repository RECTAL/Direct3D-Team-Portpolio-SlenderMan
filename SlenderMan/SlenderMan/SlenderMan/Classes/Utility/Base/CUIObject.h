#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../Base/CObject.h"
#include "../Interface/IRenderable.h"

//! UI Object
class CUIObject : public CObject,
	public IRenderable
{
protected:			// constructor, destructor

	CUIObject(void);						//! constructor
	virtual ~CUIObject(void);				//! destructor


public:			// interface

	virtual void draw(void) final override;			//interface: IRenderable
	virtual void drawUI(void) final override;		//interface: IRenderable

public:			// getter, setter
	/*******************************************/
	//getter
	/*******************************************/
	bool getVisible(void);							


	/*******************************************/
	//setter
	/*******************************************/
	virtual void setVisible(bool a_bIsVisible);
	void setColor(D3DCOLOR a_nDiffuseColor);		//! changeColor

protected:			

	virtual void doDrawUI(void);//! drawUI

protected:	

	bool m_bIsVisible = false;
	D3DCOLOR m_nDiffuseColor;
};
