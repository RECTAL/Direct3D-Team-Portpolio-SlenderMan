#include "WindowSystem_CWindowButton.h"
#include "../../Object/SpriteObject/CSpriteObject.h"

CWindowButton::CWindowButton(std::string a_stWindowName, CWindowType a_ECWindowType, SIZE a_stActiveSize)
	:CWindow(a_stWindowName,a_ECWindowType)
{
	m_stActiveSize = a_stActiveSize;
}

CWindowButton::~CWindowButton()
{
	
}

void CWindowButton::update(void)
{
	CWindow::update();
}

void CWindowButton::draw(void)
{
	CWindow::draw();
	m_pSpriteObject->drawUI();
}

void CWindowButton::init(
	std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, CSpriteObject* a_pSpriteObject)
{
	CWindow::init(a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc, a_pSpriteObject);
	this->createBeginCallBackFunc(a_pBeginCallBackFunc);
	this->createCallBackFunc(a_pCallBackFunc);
	this->createEndCallBackFunc(a_pEndCallBackFunc);

	m_pSpriteObject = a_pSpriteObject;
}

void CWindowButton::release()
{
	CWindow::release();
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

void CWindowButton::setIsActive(bool isActive, POINT a_stMousePosition)
{
	m_bPreActive = m_bIsActive;
	m_bIsActive = isActive;
	if (m_bIsActive)
	{
		m_stBeginCallBackFunc();
	}
	else
	{
		m_stEndCallBackFunc();
	}
}
