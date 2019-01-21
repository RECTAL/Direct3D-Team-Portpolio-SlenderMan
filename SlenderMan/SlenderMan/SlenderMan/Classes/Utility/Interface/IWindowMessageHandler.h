#pragma once

#include "../../Define/KGlobalDefine.h"

//! windowMessage Processer handler
class IWindowMessageHandler
{
public:			// interface

	//! process windowMessage
	virtual LRESULT handleWindowMessage(HWND a_hWindow,
		UINT a_nMessage,
		WPARAM a_wParam,
		LPARAM a_lParam) = 0;
};
