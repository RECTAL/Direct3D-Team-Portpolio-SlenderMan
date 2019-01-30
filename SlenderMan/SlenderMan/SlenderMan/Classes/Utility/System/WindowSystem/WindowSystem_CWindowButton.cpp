#include "WindowSystem_CWindowButton.h"
#include "../../Object/SpriteObject/CSpriteObject_Kind/CSpriteObject_Button.h"

CWindowButton::CWindowButton(std::string a_stWindowName, CWindowType a_ECWindowType, SIZE a_stActiveSize, D3DXVECTOR3 a_stAbsolutePos)
	:CWindow(a_stWindowName,a_ECWindowType, a_stAbsolutePos)
{
	m_stActiveSize = a_stActiveSize;
}

CWindowButton::~CWindowButton()
{
	
}

void CWindowButton::update(void)
{

	CWindow::update();
	this->createActiveRect();
}

void CWindowButton::init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc)
{
	CWindow::init(a_pCrashCallBackFunc, a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc);
	this->createCrashCallBackFunc(a_pCrashCallBackFunc);
	this->createBeginCallBackFunc(a_pBeginCallBackFunc);
	this->createCallBackFunc(a_pCallBackFunc);
	this->createEndCallBackFunc(a_pEndCallBackFunc);

	this->createActiveRect();
}

void CWindowButton::release()
{
	CWindow::release();
}

void CWindowButton::createCrashCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stCrashCallBackFunc = [=](void)->void {
			m_nButtonOffset = 2;
		};
	}
	else
	{
		m_stCrashCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowButton::createBeginCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stBeginCallBackFunc = [=](void)->void{
			m_nButtonOffset = 1;
		};
	}
	else
	{
		m_stBeginCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowButton::createCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stCallBackFunc = [=](void)->void {
		
		};
	}
	else
	{
		m_stCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowButton::createEndCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stEndCallBackFunc = [=](void)->void {
			m_nButtonOffset = 0;
		};
	}
	else
	{
		m_stEndCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowButton::createActiveRect()
{
	m_stActiveRect = RECT{
		   (LONG)(m_stAbsolutePosition.x - m_stActiveSize.cx / 2),

		   (LONG)(m_stAbsolutePosition.y - m_stActiveSize.cy / 2),
		   (LONG)(m_stAbsolutePosition.x + m_stActiveSize.cx / 2),
		   (LONG)(m_stAbsolutePosition.y + m_stActiveSize.cy / 2)
	};
}
