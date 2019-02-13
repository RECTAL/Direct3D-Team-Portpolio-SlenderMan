#include "CObject.h"

CObject::CObject(void)
	:
	m_stPosition(0.0f, 0.0f, 0.0f),
	m_stScale(1.0f, 1.0f, 1.0f),

	m_stRightDirection(WORLD_RIGHT_DIRECTION),
	m_stUpDirection(WORLD_UP_DIRECTION),
	m_stForwardDirection(WORLD_FORWARD_DIRECTION)
{
	D3DXMatrixIdentity(&m_stWorldMatrix);
	D3DXMatrixIdentity(&m_stOffsetMatrix);
}

CObject::~CObject(void)
{
	for (auto pChildObject : m_oChildObjectList) {
		SAFE_DELETE(pChildObject);
	}
}

void CObject::update(void)
{
	D3DXMATRIXA16 stTranslation;

	D3DXMatrixTranslation(&stTranslation,
		m_stPosition.x,
		m_stPosition.y,
		m_stPosition.z);

	D3DXMATRIXA16 stScale;

	D3DXMatrixScaling(&stScale,
		m_stScale.x,
		m_stScale.y,
		m_stScale.z);

	D3DXMATRIXA16 stRotation;
	D3DXMatrixIdentity(&stRotation);

	CopyMemory(&stRotation(0, 0), &m_stRightDirection, sizeof(D3DXVECTOR3));
	CopyMemory(&stRotation(1, 0), &m_stUpDirection, sizeof(D3DXVECTOR3));
	CopyMemory(&stRotation(2, 0), &m_stForwardDirection, sizeof(D3DXVECTOR3));

	m_stWorldMatrix = m_stOffsetMatrix * stScale * stRotation * stTranslation;

	for (auto pChildObject : m_oChildObjectList) {
		pChildObject->update();
	}
}

D3DXVECTOR3 CObject::getPosition(void)
{
	return m_stPosition;
}

D3DXVECTOR3 CObject::getScale(void)
{
	return m_stScale;
}

D3DXVECTOR3 CObject::getRightDirection(void)
{
	return m_stRightDirection;
}

D3DXVECTOR3 CObject::getUpDirection(void)
{
	return m_stUpDirection;
}

D3DXVECTOR3 CObject::getForwardDirection(void)
{
	return m_stForwardDirection;
}

D3DXMATRIXA16 CObject::getWorldMatrix(void)
{
	return m_stWorldMatrix;
}

D3DXMATRIXA16 CObject::getFinalWorldMatrix(void)
{
	if (m_pParentObject == nullptr) {
		return m_stWorldMatrix;
	}

	auto stParentMatrix = m_pParentObject->getFinalWorldMatrix();
	return m_stWorldMatrix * stParentMatrix;
}

void CObject::setParentObject(CObject * a_pParentObject)
{
	assert(m_pParentObject == nullptr);
	m_pParentObject = a_pParentObject;
}

void CObject::setPosition(const D3DXVECTOR3 & a_rstPosition)
{
	m_stPosition = a_rstPosition;
}

void CObject::setScale(const D3DXVECTOR3 & a_rstScale)
{
	m_stScale = a_rstScale;
}

void CObject::setRotation(const D3DXVECTOR3 & a_rstRotation)
{
	D3DXMATRIXA16 stRotation;

	D3DXMatrixRotationYawPitchRoll(&stRotation,
		D3DXToRadian(a_rstRotation.y),
		D3DXToRadian(a_rstRotation.x),
		D3DXToRadian(a_rstRotation.z));

	CopyMemory(&m_stRightDirection, &stRotation(0, 0), sizeof(D3DXVECTOR3));
	CopyMemory(&m_stUpDirection, &stRotation(1, 0), sizeof(D3DXVECTOR3));
	CopyMemory(&m_stForwardDirection, &stRotation(2, 0), sizeof(D3DXVECTOR3));
}

void CObject::setOffsetMatrix(const D3DXMATRIXA16 & a_rstOffsetMatrix)
{
	m_stOffsetMatrix = a_rstOffsetMatrix;
}

void CObject::setForwardDirection(D3DXVECTOR3 & a_stDirection)
{
	m_stForwardDirection = a_stDirection;
}

void CObject::setUpDirection(D3DXVECTOR3 & a_stDirection)
{
	m_stUpDirection = a_stDirection;
}

void CObject::setRightDirection(D3DXVECTOR3 & a_stDirection)
{
	m_stRightDirection = a_stDirection;
}

void CObject::resetAxis(void)
{
	D3DXVec3Normalize(&m_stRightDirection, &m_stRightDirection);
	D3DXVec3Normalize(&m_stUpDirection, &m_stUpDirection);
	D3DXVec3Normalize(&m_stForwardDirection, &m_stForwardDirection);
}

void CObject::moveByXAxis(float a_fDistance, bool a_bIsLocal)
{
	if (a_bIsLocal) {
		m_stPosition += m_stRightDirection * a_fDistance;
	}
	else {
		m_stPosition += WORLD_RIGHT_DIRECTION * a_fDistance;
	}
}

void CObject::moveByYAxis(float a_fDistance, bool a_bIsLocal)
{
	if (a_bIsLocal) {
		m_stPosition += m_stUpDirection * a_fDistance;
	}
	else {
		m_stPosition += WORLD_UP_DIRECTION * a_fDistance;
	}
}

void CObject::moveByZAxis(float a_fDistance, bool a_bIsLocal)
{
	if (a_bIsLocal) {
		m_stPosition += m_stForwardDirection * a_fDistance;
	}
	else {
		m_stPosition += WORLD_FORWARD_DIRECTION * a_fDistance;
	}
}

void CObject::rotateByXAxis(float a_fAngle, bool a_bIsLocal)
{
	D3DXMATRIXA16 stRotation;

	if (a_bIsLocal) {
		D3DXMatrixRotationAxis(&stRotation, &m_stRightDirection, D3DXToRadian(a_fAngle));
	}
	else {
		D3DXMatrixRotationAxis(&stRotation, &WORLD_RIGHT_DIRECTION, D3DXToRadian(a_fAngle));
	}

	this->rotateByMatrix(stRotation);
}

void CObject::rotateByYAxis(float a_fAngle, bool a_bIsLocal)
{
	D3DXMATRIXA16 stRotation;

	if (a_bIsLocal) {
		D3DXMatrixRotationAxis(&stRotation, &m_stUpDirection, D3DXToRadian(a_fAngle));
	}
	else {
		D3DXMatrixRotationAxis(&stRotation, &WORLD_UP_DIRECTION, D3DXToRadian(a_fAngle));
	}

	this->rotateByMatrix(stRotation);
}

void CObject::rotateByZAxis(float a_fAngle, bool a_bIsLocal)
{
	D3DXMATRIXA16 stRotation;

	if (a_bIsLocal) {
		D3DXMatrixRotationAxis(&stRotation, &m_stForwardDirection, D3DXToRadian(a_fAngle));
	}
	else {
		D3DXMatrixRotationAxis(&stRotation, &WORLD_FORWARD_DIRECTION, D3DXToRadian(a_fAngle));
	}

	this->rotateByMatrix(stRotation);
}

void CObject::rotateByPosition(const D3DXVECTOR3 & a_rstPosition, bool a_bIsSphere)
{
	D3DXVECTOR3 stDirection = a_rstPosition - m_stPosition;
	D3DXVec3Normalize(&stDirection, &stDirection);

	this->rotateByDirection(stDirection, a_bIsSphere);
}

void CObject::rotateByDirection(const D3DXVECTOR3 & a_rstDirection, bool a_bIsSphere)
{
	m_stForwardDirection = a_rstDirection;

	if (!a_bIsSphere) {
		m_stForwardDirection.y = 0.0f;
	}

	D3DXVec3Cross(&m_stRightDirection, &m_stUpDirection, &m_stForwardDirection);
	D3DXVec3Cross(&m_stUpDirection, &m_stForwardDirection, &m_stRightDirection);

	this->resetAxis();
}

void CObject::rotateByMatrix(const D3DXMATRIXA16 & a_rstMatrix)
{
	D3DXVec3TransformNormal(&m_stRightDirection, &m_stRightDirection, &a_rstMatrix);
	D3DXVec3TransformNormal(&m_stUpDirection, &m_stUpDirection, &a_rstMatrix);
	D3DXVec3TransformNormal(&m_stForwardDirection, &m_stForwardDirection, &a_rstMatrix);

	this->resetAxis();
}

void CObject::addChildObject(CObject * a_pChildObject)
{
	auto oIterator = std::find(m_oChildObjectList.begin(),
		m_oChildObjectList.end(),
		a_pChildObject);

	if (oIterator == m_oChildObjectList.end()) {
		a_pChildObject->setParentObject(this);
		m_oChildObjectList.push_back(a_pChildObject);
	}
}

void CObject::removeChildObject(CObject * a_pChildObject)
{
	auto oIterator = std::find(m_oChildObjectList.begin(),
		m_oChildObjectList.end(), a_pChildObject);

	if (oIterator != m_oChildObjectList.end()) {
		m_oChildObjectList.erase(oIterator);
	}
}
