#include "SpotLightObject.h"


CSpotLightObject::CSpotLightObject(int a_nIndex, float a_fRange, float a_fTheta, float a_fPhi)
	:CLightObject(a_nIndex)
{
	m_stLight.Type = D3DLIGHT_SPOT;
	m_stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_stLight.Attenuation0 = 0.0000001f;
	m_stLight.Attenuation1 = 0.0000001f;
	m_stLight.Attenuation2 = 0.0000001f;
	m_stLight.Falloff = 1.0f;
	m_stLight.Range = a_fRange;
	m_stLight.Theta = a_fTheta;
	m_stLight.Phi = a_fPhi;


	m_fRange	= a_fRange;
	m_fTheta	= a_fTheta;
	m_fPhi		= a_fPhi;

}

void CSpotLightObject::update(void)
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
