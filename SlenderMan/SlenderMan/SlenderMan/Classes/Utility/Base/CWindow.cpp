#include "CWindow.h"
#include "../Object/SpriteObject/CSpriteObject.h"
CWindow::CWindow(std::string a_stWindowName, CWindowType a_ECWindowType, D3DXVECTOR3 a_stAbsolutePos)
	:m_stWindowName(a_stWindowName),m_eCWindowType(a_ECWindowType),m_stAbsolutePosition(a_stAbsolutePos)
{
}

CWindow::~CWindow()
{
	this->release();
}

void CWindow::update(void)
{
	m_stActiveRect = RECT{
		(LONG)(m_stAbsolutePosition.x - m_stActiveSize.cx / 2),
		(LONG)(m_stAbsolutePosition.y - m_stActiveSize.cy / 2),
		(LONG)(m_stAbsolutePosition.x + m_stActiveSize.cx / 2),
		(LONG)(m_stAbsolutePosition.y + m_stActiveSize.cy / 2)
	};
	D3DXVECTOR3 stOffset = m_stAbsolutePosition;
	for (auto oIterator : m_oChildWindow)
	{
		D3DXVECTOR3 stRelativePos = oIterator->getRelativePosition();
		oIterator->setAbsolutePosition(stOffset + stRelativePos);
	}

	for (auto oIterator : m_oChildWindow)
	{
		oIterator->update();
	}
}


void CWindow::init(std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc)
{
}

void CWindow::release()
{
	for (auto oIterator : m_oChildWindow)
	{
		oIterator->release();
	}
}
