#include "WindowSystem_CWindowContainer.h"
#include "../../Manager/CInputManager.h"
#include "../../Object/SpriteObject/CSpriteObject.h"
#include "../../Manager/CInputManager.h"
#include "../../Manager/CTimeManager.h"

CWindowContainer::CWindowContainer(std::string a_stWindowName, CWindowType a_ECWindowType, SIZE a_stActiveSize, D3DXVECTOR3 a_stAbsolutePos)
	:CWindow(a_stWindowName,a_ECWindowType, a_stAbsolutePos)
{
	m_stActiveSize = a_stActiveSize;
}

CWindowContainer::~CWindowContainer()
{
}

void CWindowContainer::update(void)
{
	CWindow::update();
	this->createActiveRect();
}
void CWindowContainer::init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc,
	std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc)
{
	CWindow::init(a_pCrashCallBackFunc, a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc);
	this->createBeginCallBackFunc(a_pBeginCallBackFunc);
	this->createCallBackFunc(a_pCallBackFunc);
	this->createEndCallBackFunc(a_pEndCallBackFunc);
	this->createActiveRect();
}

void CWindowContainer::release()
{
	CWindow::release();
}

void CWindowContainer::createCrashCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stCrashCallBackFunc = [=](void)->void
		{
			printf("CrashCallBackFunc");
		};
	}
	else
	{
		m_stCrashCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowContainer::createBeginCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stBeginCallBackFunc = [=](void)->void
		{
			m_stPreOffset = GET_MOUSE_POSITION();
			m_stDeltaX = 0;
			m_stDeltaY = 0;
		};
	}
	else
	{
		m_stBeginCallBackFunc = (*a_pCallBackFunc);
	}
}


void CWindowContainer::createCallBackFunc(std::function<void(void)>* a_pCallBackFunction)
{
	if (a_pCallBackFunction == nullptr)
	{
		m_stCallBackFunc = [=](void)->void {
		
			m_stOffset = GET_MOUSE_POSITION();
			m_stDeltaX = m_stOffset.x - m_stPreOffset.x;
			m_stDeltaY = m_stOffset.y - m_stPreOffset.y;

			m_stAbsolutePosition.x += m_stDeltaX;
			m_stAbsolutePosition.y += m_stDeltaY;

			m_stPreOffset.x = m_stOffset.x;
			m_stPreOffset.y = m_stOffset.y;
		};
	}
	else
	{
		m_stCallBackFunc = (*a_pCallBackFunction);
	}
}

void CWindowContainer::createEndCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stEndCallBackFunc = [=](void)->void
		{
			m_stDeltaX = 0;
			m_stDeltaY = 0;
		};
	}
	else
	{
		m_stEndCallBackFunc = (*a_pCallBackFunc);
	}
}

void CWindowContainer::createActiveRect()
{
	m_stActiveRect = RECT{
		   (LONG)(m_stAbsolutePosition.x - m_stActiveSize.cx / 2 - abs(m_stDeltaX) * 300),
		   (LONG)(m_stAbsolutePosition.y - m_stActiveSize.cy / 2 - abs(m_stDeltaY) * 600),
		   (LONG)(m_stAbsolutePosition.x + m_stActiveSize.cx / 2 + abs(m_stDeltaX) * 300),
		   (LONG)(m_stAbsolutePosition.y - m_stActiveSize.cy / 2 + 50 + abs(m_stDeltaY) * 600)
	};
}

