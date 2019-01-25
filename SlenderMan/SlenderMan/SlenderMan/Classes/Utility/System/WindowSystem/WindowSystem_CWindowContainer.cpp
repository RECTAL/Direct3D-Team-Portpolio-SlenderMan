#include "WindowSystem_CWindowContainer.h"
#include "../../Manager/CInputManager.h"
#include "../../Object/SpriteObject/CSpriteObject.h"
#include "../../Manager/CInputManager.h"

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
}



void CWindowContainer::init(
	std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc)
{
	CWindow::init(a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc);
	this->createBeginCallBackFunc(a_pBeginCallBackFunc);
	this->createCallBackFunc(a_pCallBackFunc);
	this->createEndCallBackFunc(a_pEndCallBackFunc);


}

void CWindowContainer::release()
{
	CWindow::release();
}

void CWindowContainer::createBeginCallBackFunc(std::function<void(void)>* a_pCallBackFunc)
{
	if (a_pCallBackFunc == nullptr)
	{
		m_stBeginCallBackFunc = [=](void)->void
		{
			m_stPreOffset = GET_MOUSE_POSITION();
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
			LONG stDeltaX = m_stOffset.x - m_stPreOffset.x;
			LONG stDeltaY = m_stOffset.y - m_stPreOffset.y;

			m_stAbsolutePosition.x += stDeltaX;
			m_stAbsolutePosition.y += stDeltaY;

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
			
		};
	}
	else
	{
		m_stEndCallBackFunc = (*a_pCallBackFunc);
	}
}

