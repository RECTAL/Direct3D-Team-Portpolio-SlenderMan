#include "WindowSystem_CWindowContainer.h"
#include "../../Manager/CInputManager.h"
#include "../../Object/SpriteObject/CSpriteObject.h"
CWindowContainer::CWindowContainer(std::string a_stWindowName, CWindowType a_ECWindowType, SIZE a_stActiveSize)
	:CWindow(a_stWindowName,a_ECWindowType)
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

void CWindowContainer::draw(void)
{
	CWindow::draw();
	m_pSpriteObject->drawUI();
}

void CWindowContainer::init(
	std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, CSpriteObject* a_pSpriteObject)
{
	CWindow::init(a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc, a_pSpriteObject);
	this->createBeginCallBackFunc(a_pBeginCallBackFunc);
	this->createCallBackFunc(a_pCallBackFunc);
	this->createEndCallBackFunc(a_pEndCallBackFunc);

	m_pSpriteObject = a_pSpriteObject;
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
		//	m_stPreOffset = GET_MOUSE_POSITION();
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
		
			//m_stOffset = GET_MOUSE_POSITION();
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

void CWindowContainer::setIsActive(bool isActive, POINT a_stMousePosition)
{
	m_bPreActive = m_bIsActive;
	m_bIsActive = isActive;
	m_stPreOffset = a_stMousePosition;
	if (m_bIsActive)
	{
		(m_stBeginCallBackFunc)();
	}
	else
	{
		(m_stEndCallBackFunc)();
	}

}
