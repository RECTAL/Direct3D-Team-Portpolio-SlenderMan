#include "CLightObject.h"
#include "../../Manager/CDeviceManager.h"

CLightObject::CLightObject(int a_nIndex)
	:
	m_nIndex(a_nIndex)
{
	ZeroMemory(&m_stLight, sizeof(m_stLight));

	m_stLight.Type = D3DLIGHT_DIRECTIONAL;
	m_stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

CLightObject::~CLightObject()
{
}

bool CLightObject::getLightEnable(void)
{
	return m_bIsEnable;
}

D3DXMATRIXA16 CLightObject::getViewMatrix(void)
{
	return m_stViewMatrix;
}

void CLightObject::setLightEnable(bool a_bIsEnable)
{
	m_bIsEnable = a_bIsEnable;
	GET_DEVICE()->SetLight(m_nIndex, &m_stLight);
	GET_DEVICE()->LightEnable(m_nIndex, m_bIsEnable);
}

void CLightObject::setDiffuseColor(const D3DXCOLOR & a_rstColor)
{
	m_stLight.Diffuse = a_rstColor;
}

void CLightObject::update(void)
{
	CObject::update();
	m_stLight.Direction = this->getForwardDirection();

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
}