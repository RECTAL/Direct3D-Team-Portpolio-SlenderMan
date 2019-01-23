#include "CWindow.h"
#include "../Object/SpriteObject/CSpriteObject.h"
CWindow::CWindow(std::string a_stWindowName, CWindowType a_ECWindowType)
	:m_stWindowName(a_stWindowName),m_eCWindowType(a_ECWindowType)
{
}

CWindow::~CWindow()
{
	this->release();
}

void CWindow::update(void)
{
	m_pSpriteObject->update();
	if (m_bIsActive&&m_bPreActive)
	{
		m_stCallBackFunc();
	}
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

void CWindow::draw(void)
{

}

void CWindow::init(std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, CSpriteObject* a_pSpriteObject)
{
}

void CWindow::release()
{
	SAFE_DELETE(m_pSpriteObject);
	for (auto oIterator : m_oChildWindow)
	{
		oIterator->release();
	}
}
