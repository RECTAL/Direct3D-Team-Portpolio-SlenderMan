#include "CRenderObject.h"

CRenderObject::CRenderObject(void)
	:
	m_bIsVisible(true)
{
	// Do Nothing
}

void CRenderObject::draw(void)
{
	if (m_bIsVisible) {
		this->preDraw();
		this->doDraw();
		this->postDraw();
	}

	for (auto pChildObject : m_oChildObjectList) {
		auto pRenderObject = dynamic_cast<CRenderObject *>(pChildObject);

		if (pRenderObject != nullptr) {
			pRenderObject->draw();
		}
	}
}

bool CRenderObject::getVisible(void)
{
	return m_bIsVisible;
}

void CRenderObject::setVisible(bool a_bIsVisible)
{
	m_bIsVisible = a_bIsVisible;
}

void CRenderObject::preDraw(void)
{
	// Do Nothing
}

void CRenderObject::doDraw(void)
{
	// Do Nothing
}

void CRenderObject::postDraw(void)
{
	// Do Nothing
}
