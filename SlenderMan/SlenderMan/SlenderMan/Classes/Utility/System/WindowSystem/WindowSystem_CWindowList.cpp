#include "WindowSystem_CWindowList.h"

CWindowList::CWindowList(std::string a_stWindowName, CWindowType a_ECWindowType, SIZE a_stActiveSize, D3DXVECTOR3 a_stAbsolutePos)
	:CWindow(a_stWindowName,a_ECWindowType,a_stAbsolutePos)
{
	m_stActiveSize = a_stActiveSize;
}

CWindowList::~CWindowList()
{
}

void CWindowList::update(void)
{
	CWindow::update();
	this->createActiveRect();
}

void CWindowList::init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc)
{
	CWindow::init(a_pCrashCallBackFunc, a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc);
	this->createBeginCallBackFunc(a_pBeginCallBackFunc);
	this->createCallBackFunc(a_pCallBackFunc);
	this->createEndCallBackFunc(a_pEndCallBackFunc);

	this->createActiveRect();
}

void CWindowList::release()
{
}

void CWindowList::createCrashCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stCrashCallBackFunc = [=](void)->void
		{
			printf("Crash CallbackFunction");
		};
	}
	else
	{
		m_stCrashCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowList::createBeginCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stBeginCallBackFunc = [=](void)->void
		{
			printf("begin CallbackFunction");
		};
	}
	else
	{
		m_stBeginCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowList::createCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stCallBackFunc = [=](void)->void
		{
			printf("CallbackFunction");
		};
	}
	else
	{
		m_stCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowList::createEndCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stEndCallBackFunc = [=](void)->void
		{
			printf("end CallbackFunction");
		};
	}
	else
	{
		m_stEndCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowList::createActiveRect()
{
	m_stActiveRect = RECT{
		(LONG)(m_stAbsolutePosition.x - m_stActiveSize.cx / 2),
		(LONG)(m_stAbsolutePosition.y - m_stActiveSize.cy / 2),
		(LONG)(m_stAbsolutePosition.x + m_stActiveSize.cx / 2),
		(LONG)(m_stAbsolutePosition.y + m_stActiveSize.cy / 2)
	};
}
