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

STBoundingSphere CRenderObject::getFinalBoundingSphere()
{
	STBoundingSphere stBoundingSphere;
	D3DXMATRIXA16 stWorldMatrix = this->getFinalWorldMatrix();

	D3DXVec3TransformCoord(&stBoundingSphere.m_stPosition,
		&m_stBoundingSphere.m_stPosition, &stWorldMatrix);

	stBoundingSphere.m_fRadius = m_stBoundingSphere.m_fRadius * max(max(stWorldMatrix(0, 0), stWorldMatrix(1, 1)),
		stWorldMatrix(2, 2));

	return stBoundingSphere;
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
