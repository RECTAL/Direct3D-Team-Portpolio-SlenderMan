#include "WindowSystem_CWindowScrollBar.h"

CWindowScrollBar::CWindowScrollBar(std::string a_stWindowName, CWindowType a_ECWindowType, SIZE a_stActiveSize, D3DXVECTOR3 a_stAbsolutePos)
	:CWindow(a_stWindowName,a_ECWindowType,a_stAbsolutePos)
{
	m_stActiveSize = a_stActiveSize;
}

CWindowScrollBar::~CWindowScrollBar()
{
}

void CWindowScrollBar::update(void)
{
	CWindow::update();
	this->createActiveRect();
}

void CWindowScrollBar::init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc)
{
	CWindow::init(a_pCrashCallBackFunc, a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc);
	this->createBeginCallBackFunc(a_pBeginCallBackFunc);
	this->createCallBackFunc(a_pCallBackFunc);
	this->createEndCallBackFunc(a_pEndCallBackFunc);

	this->createActiveRect();
}

void CWindowScrollBar::release()
{
	CWindow::release();
}

void CWindowScrollBar::createCrashCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stCrashCallBackFunc = [=](void)->void {
			printf("ScrollBar crashCallBackFunc\n");
		};
	}
	else
	{
		m_stCrashCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowScrollBar::createBeginCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stBeginCallBackFunc = [=](void)->void {
			printf("ScrollBar beginCallBackFunc\n");
		};
	}
	else
	{
		m_stBeginCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowScrollBar::createCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stCallBackFunc = [=](void)->void {
			printf("ScrollBar callBackFunc\n");
		};
	}
	else
	{
		m_stCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowScrollBar::createEndCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stEndCallBackFunc = [=](void)->void {
			printf("ScrollBar endCallBackFunc\n");
		};
	}
	else
	{
		m_stEndCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowScrollBar::createActiveRect()
{
	m_stActiveRect = RECT{
		(LONG)(m_stAbsolutePosition.x - m_stActiveSize.cx / 2),
		(LONG)(m_stAbsolutePosition.y - m_stActiveSize.cy / 2),
		(LONG)(m_stAbsolutePosition.x + m_stActiveSize.cx / 2),
		(LONG)(m_stAbsolutePosition.y + m_stActiveSize.cy / 2)
	};
}
