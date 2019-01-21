#include "CInputManager.h"
#include "CWindowManager.h"

CInputManager::CInputManager(void)
{
	ZeroMemory(m_anKeyboardStates, sizeof(m_anKeyboardStates));
	ZeroMemory(m_anPrevKeyboardStates, sizeof(m_anPrevKeyboardStates));
}

CInputManager::~CInputManager(void)
{
	m_pKeyboardDevice->Unacquire();

	SAFE_RELEASE(m_pKeyboardDevice);
	SAFE_RELEASE(m_pDirectInput);
}

void CInputManager::init(void)
{
	m_pDirectInput = this->createDirectInput();
	m_pKeyboardDevice = this->createKeyboardDevice();
	m_pMouseDevice = this->createMouseDevice();
}

void CInputManager::update(void)
{
	CopyMemory(&m_stPrevMouseState, &m_stMouseState, sizeof(m_stMouseState));
	CopyMemory(m_anPrevKeyboardStates, m_anKeyboardStates, sizeof(m_anKeyboardStates));

	m_pMouseDevice->GetDeviceState(sizeof(m_stMouseState), &m_stMouseState);
	m_pKeyboardDevice->GetDeviceState(sizeof(m_anKeyboardStates), m_anKeyboardStates);
}

POINT CInputManager::getMousePosition(void)
{
	POINT stMousePosition;
	GetCursorPos(&stMousePosition);
	ScreenToClient(GET_WINDOW_HANDLE(), &stMousePosition);

	return stMousePosition;
}

bool CInputManager::isKeyDown(int a_nKeyCode)
{
	return m_anKeyboardStates[a_nKeyCode] & 0x80;
}

bool CInputManager::isKeyPressed(int a_nKeyCode)
{
	return (m_anKeyboardStates[a_nKeyCode] & 0x80) &&
		!(m_anPrevKeyboardStates[a_nKeyCode] & 0x80);
}

bool CInputManager::isKeyReleased(int a_nKeyCode)
{
	return !(m_anKeyboardStates[a_nKeyCode] & 0x80) &&
		(m_anPrevKeyboardStates[a_nKeyCode] & 0x80);
}

bool CInputManager::isMouseButtonDown(EMouseInput a_eMouseInput)
{
	int nMouseInput = (int)a_eMouseInput;
	return m_stMouseState.rgbButtons[nMouseInput] & 0x80;
}

bool CInputManager::isMouseButtonPressed(EMouseInput a_eMouseInput)
{
	int nMouseInput = (int)a_eMouseInput;

	return (m_stMouseState.rgbButtons[nMouseInput] & 0x80) &&
		!(m_stPrevMouseState.rgbButtons[nMouseInput] & 0x80);
}

bool CInputManager::isMouseButtonReleased(EMouseInput a_eMouseInput)
{
	int nMouseInput = (int)a_eMouseInput;

	return !(m_stMouseState.rgbButtons[nMouseInput] & 0x80) &&
		(m_stPrevMouseState.rgbButtons[nMouseInput] & 0x80);
}

LPDIRECTINPUT8 CInputManager::createDirectInput(void)
{
	LPDIRECTINPUT8 pDirectInput = nullptr;
	DirectInput8Create(GET_INSTANCE_HANDLE(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&pDirectInput, NULL);

	return pDirectInput;
}

LPDIRECTINPUTDEVICE8 CInputManager::createKeyboardDevice(void)
{
	LPDIRECTINPUTDEVICE8 pDevice = nullptr;
	m_pDirectInput->CreateDevice(GUID_SysKeyboard, &pDevice, NULL);

	pDevice->SetDataFormat(&c_dfDIKeyboard);

	pDevice->SetCooperativeLevel(GET_WINDOW_HANDLE(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	pDevice->Acquire();

	return pDevice;
}

LPDIRECTINPUTDEVICE8 CInputManager::createMouseDevice(void)
{
	LPDIRECTINPUTDEVICE8 pDevice = nullptr;
	m_pDirectInput->CreateDevice(GUID_SysMouse, &pDevice, NULL);

	pDevice->SetDataFormat(&c_dfDIMouse);
	pDevice->SetCooperativeLevel(GET_WINDOW_HANDLE(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	pDevice->Acquire();

	return pDevice;
}
