#include "CCameraObject.h"
#include "../../Manager/CDeviceManager.h"
#include "../../System/CollisionSystem/CollisionSystem_CFrustum.h"

CCameraObject::CCameraObject(float a_fAspect)
{
	D3DXMatrixIdentity(&m_stViewMatrix);
	D3DXMatrixIdentity(&m_stProjectionMatrix);

	this->setAspect(a_fAspect);

	m_pFrustum = new CFrustum();
}

D3DXMATRIXA16 CCameraObject::getViewMatrix(void)
{
	return m_stViewMatrix;
}

D3DXMATRIXA16 CCameraObject::getProjectionMatrix(void)
{
	return m_stProjectionMatrix;
}

void CCameraObject::setAspect(float a_fAspect)
{
	D3DXMatrixPerspectiveFovLH(&m_stProjectionMatrix,
		D3DXToRadian(50.0f),
		a_fAspect,
		0.1f,
		150.0f);

	// 투영 행렬을 설정한다
	GET_DEVICE()->SetTransform(D3DTS_PROJECTION, &m_stProjectionMatrix);
}

void CCameraObject::update(void)
{
	CObject::update();
	D3DXMatrixIdentity(&m_stViewMatrix);

	m_stViewMatrix(0, 0) = m_stRightDirection.x;
	m_stViewMatrix(1, 0) = m_stRightDirection.y;
	m_stViewMatrix(2, 0) = m_stRightDirection.z;
	m_stViewMatrix(3, 0) = -D3DXVec3Dot(&m_stPosition, &m_stRightDirection);

	m_stViewMatrix(0, 1) = m_stUpDirection.x;
	m_stViewMatrix(1, 1) = m_stUpDirection.y;
	m_stViewMatrix(2, 1) = m_stUpDirection.z;
	m_stViewMatrix(3, 1) = -D3DXVec3Dot(&m_stPosition, &m_stUpDirection);

	m_stViewMatrix(0, 2) = m_stForwardDirection.x;
	m_stViewMatrix(1, 2) = m_stForwardDirection.y;
	m_stViewMatrix(2, 2) = m_stForwardDirection.z;
	m_stViewMatrix(3, 2) = -D3DXVec3Dot(&m_stPosition, &m_stForwardDirection);

	// 뷰 행렬을 설정한다
	GET_DEVICE()->SetTransform(D3DTS_VIEW, &m_stViewMatrix);

	D3DXMATRIXA16 stViewProj = m_stViewMatrix * m_stProjectionMatrix;
	m_pFrustum->Make(&stViewProj);
}
