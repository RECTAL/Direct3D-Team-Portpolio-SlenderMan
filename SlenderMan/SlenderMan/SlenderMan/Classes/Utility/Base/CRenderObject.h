#pragma once

#include "../../Define/KGlobalDefine.h"
#include "CObject.h"
#include "../Interface/IRenderable.h"

//! RenderObject
class CRenderObject : public CObject,
	public IRenderable
{
protected:		// constructor
	CRenderObject(void);

public:			// interface
	virtual void draw(void) final override;			//interface: IRenderable


public:			// getter, setter
	/**********************************************/
	//getter
	/**********************************************/
	bool getVisible(void);							//! get IsVisible

	/**********************************************/
	//setter
	/**********************************************/
	void setVisible(bool a_bIsVisible);				//! set IsVisible

protected:		

	virtual void preDraw(void);						//! Before Drawing
	virtual void doDraw(void);						//! Drawing
	virtual void postDraw(void);					//! After Drawed


protected:	

	bool m_bIsVisible = false;
};
