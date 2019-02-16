#include "CRenderObject.h"
#include "../Debug/CDebugDraw.h"

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

		// 자식 객체를 그린다
		for (auto pChildObject : m_oChildObjectList) {
			auto pRenderObject = dynamic_cast<CRenderObject *>(pChildObject);

			if (pRenderObject != nullptr) {
				pRenderObject->draw();
			}
		}
	}
}

bool CRenderObject::getVisible(void)
{
	return m_bIsVisible;
}

STBoundingBox CRenderObject::getBoundingBox(void)
{
	return m_stBoundingBox;
}

STObjectBox CRenderObject::getObjectBox(void)
{
	STObjectBox stObjectBox;
	ZeroMemory(&stObjectBox, sizeof(stObjectBox));

	auto stBoundingBox = this->getFinalBoundingBox();
	auto stWorldMatrix = this->getFinalWorldMatrix();

	// 중점 정보를 설정한다
	stObjectBox.m_stCenter = (stBoundingBox.m_stMinPoint + stBoundingBox.m_stMaxPoint) / 2.0f;

	// 축 정보를 설정한다
	// {
	CopyMemory(stObjectBox.m_astDirections + 0, &stWorldMatrix(0, 0), sizeof(D3DXVECTOR3));
	CopyMemory(stObjectBox.m_astDirections + 1, &stWorldMatrix(1, 0), sizeof(D3DXVECTOR3));
	CopyMemory(stObjectBox.m_astDirections + 2, &stWorldMatrix(2, 0), sizeof(D3DXVECTOR3));

	D3DXVec3Normalize(stObjectBox.m_astDirections + 0, stObjectBox.m_astDirections + 0);
	D3DXVec3Normalize(stObjectBox.m_astDirections + 1, stObjectBox.m_astDirections + 1);
	D3DXVec3Normalize(stObjectBox.m_astDirections + 2, stObjectBox.m_astDirections + 2);
	// }

	// 길이 정보를 설정한다
	// {
	for (int i = 0; i < 3; ++i) {
		auto stDistance = stBoundingBox.m_stMaxPoint - stBoundingBox.m_stMinPoint;
		auto stWorldDirection = stObjectBox.m_astDirections[i];

		float fDotValue = D3DXVec3Dot(&stDistance, &stWorldDirection) / 2.0f;
		stObjectBox.m_fHalfLengths[i] = fabsf(fDotValue);
	}
	// }

	return stObjectBox;
}

STBoundingBox CRenderObject::getFinalBoundingBox(void)
{
	STBoundingBox stBoundingBox;
	D3DXMATRIXA16 stWorldMatrix = this->getFinalWorldMatrix();

	D3DXVec3TransformCoord(&stBoundingBox.m_stMinPoint,
		&m_stBoundingBox.m_stMinPoint, &stWorldMatrix);

	D3DXVec3TransformCoord(&stBoundingBox.m_stMaxPoint,
		&m_stBoundingBox.m_stMaxPoint, &stWorldMatrix);

	return stBoundingBox;
}

STBoundingSphere CRenderObject::getBoundingSphere(void)
{
	return m_stBoundingSphere;
}

STBoundingSphere CRenderObject::getFinalBoundingSphere(void)
{
	STBoundingSphere stBoundingSphere;
	D3DXMATRIXA16 stWorldMatrix = this->getFinalWorldMatrix();

	D3DXVec3TransformCoord(&stBoundingSphere.m_stCenter,
		&m_stBoundingSphere.m_stCenter, &stWorldMatrix);

	stBoundingSphere.m_fRadius = m_stBoundingSphere.m_fRadius * max(max(stWorldMatrix(0, 0), stWorldMatrix(1, 1)),
		stWorldMatrix(2, 2));

	return stBoundingSphere;
}

void CRenderObject::setVisible(bool a_bIsVisible)
{
	m_bIsVisible = a_bIsVisible;
}

void CRenderObject::setDebugEnable(bool a_bIsDebugEnable, EDebugDrawType a_eDebugDrawType)
{
	if (m_pDebugDraw != nullptr)m_pDebugDraw->setbDraw(m_bIsCollision);
	if (m_bIsDebugEnable != a_bIsDebugEnable) {
		m_bIsDebugEnable = a_bIsDebugEnable;

		if (!m_bIsDebugEnable) {
			this->removeChildObject(m_pDebugDraw);
			SAFE_DELETE(m_pDebugDraw);
		}
		else {
			m_pDebugDraw = new CDebugDraw(this, a_eDebugDrawType,m_bIsCollision);
			m_pDebugDraw->setPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			this->addChildObject(m_pDebugDraw);
		}
	}
}

void CRenderObject::setBoundingBox(const STBoundingBox & a_rstBoundingBox)
{
	m_stBoundingBox = a_rstBoundingBox;
}

void CRenderObject::setBoundingSphere(const STBoundingSphere & a_rstBoundingSphere)
{
	m_stBoundingSphere = a_rstBoundingSphere;
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
