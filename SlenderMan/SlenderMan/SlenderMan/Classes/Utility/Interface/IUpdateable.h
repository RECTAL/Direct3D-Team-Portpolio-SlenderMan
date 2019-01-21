#pragma once

#include "../../Define/KGlobalDefine.h"

//! update Interface
class IUpdateable
{
public:			// Interface

	//! update state
	virtual void update(void) = 0;
};
