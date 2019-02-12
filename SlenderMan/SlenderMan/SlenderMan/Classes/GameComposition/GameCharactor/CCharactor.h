#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../../Utility/Base/CRenderObject.h"

class CCharactor : public CRenderObject
{
public:
	
	virtual void preDraw(void) override;					//! Before Drawing
	virtual void doDraw(void) override;						//! Drawing
	virtual void postDraw(void) override;					//! After Drawed


protected:
	STRay m_stRay;


};