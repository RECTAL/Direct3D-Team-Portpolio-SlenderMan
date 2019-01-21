#pragma once

#include "../../Define/KGlobalDefine.h"

//! rendering Interface
class IRenderable
{
public:			// Interface

	//! draw (Require)
	virtual void draw(void) = 0;

	//! drawUI (Optional)
	virtual void drawUI(void) { }
};
