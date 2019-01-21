#include "CUIObject.h"

CUIObject::CUIObject(void)
	:
	m_bIsVisible(true),
	m_nDiffuseColor(D3DCOLOR_XRGB(255, 255, 255))
{
	// Do Nothing
}

CUIObject::~CUIObject(void)
{
	// Do Nothing
}

void CUIObject::draw(void)
{
	// Do Nothing
}

void CUIObject::drawUI(void)
{
	if (m_bIsVisible) {
		this->doDrawUI();
	}

	for (auto pChildObject : m_oChildObjectList) {
		auto pRenderObject = dynamic_cast<CUIObject *>(pChildObject);

		if (pRenderObject != nullptr) {
			pRenderObject->drawUI();
		}
	}
}

bool CUIObject::getVisible(void)
{
	return m_bIsVisible;
}

void CUIObject::setVisible(bool a_bIsVisible)
{
	m_bIsVisible = a_bIsVisible;
}

void CUIObject::setColor(D3DCOLOR a_nDiffuseColor)
{
	m_nDiffuseColor = a_nDiffuseColor;
}

void CUIObject::doDrawUI(void)
{
	// Do Nothing
}
