#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../Interface/IUpdateable.h"

//! Input Manager
class CInputManager : public IUpdateable
{

private:			// constructor, destructor

	//! constructor
	CInputManager(void);

	//! destructor
	virtual ~CInputManager(void);


public:			// interface

	//! interface: IUpdateable
	virtual void update(void) override;

public:			// getter

	//! get MousePosition
	POINT getMousePosition(void);

public:			// public ÇÔ¼ö

	//! singleton declare
	DECLARE_SINGLETON(CInputManager);

	//! initialization
	void init(void);

	//! check key Down
	bool isKeyDown(int a_nKeyCode);

	//! check key Pressed
	bool isKeyPressed(int a_nKeyCode);

	//! check key Released
	bool isKeyReleased(int a_nKeyCode);

	//! check MouseButton Down
	bool isMouseButtonDown(EMouseInput a_eMouseInput);

	//! check MouseButton Pressed
	bool isMouseButtonPressed(EMouseInput a_eMouseInput);

	//! check MouseButton Released
	bool isMouseButtonReleased(EMouseInput a_eMouseInput);

private:			

	//! createDirectInput8
	LPDIRECTINPUT8 createDirectInput(void);

	//! create KeyboardDevice
	LPDIRECTINPUTDEVICE8 createKeyboardDevice(void);

	//! create MouseDevice
	LPDIRECTINPUTDEVICE8 createMouseDevice(void);

private:			

	BYTE m_anKeyboardStates[UCHAR_MAX + 1];
	BYTE m_anPrevKeyboardStates[UCHAR_MAX + 1];

	DIMOUSESTATE m_stMouseState;
	DIMOUSESTATE m_stPrevMouseState;

	LPDIRECTINPUT8 m_pDirectInput = nullptr;
	LPDIRECTINPUTDEVICE8 m_pKeyboardDevice = nullptr;
	LPDIRECTINPUTDEVICE8 m_pMouseDevice = nullptr;
};
