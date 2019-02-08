#include "GlobalFunction.h"
#include "../Utility/Manager/CDeviceManager.h"

DWORD FloatToDWORD(float a_fValue)
{
	float *pnValue = &a_fValue;
	return *((DWORD *)pnValue);
	return 0;
}

int RandomIntValue(int a_nMin, int a_nMax)
{
	std::random_device oRandomDevice;
	std::uniform_int_distribution<int> oUniformRandom(a_nMin, a_nMax);

	return oUniformRandom(oRandomDevice);
}

float RandomFloatValue(float a_fMin, float a_fMax)
{
	std::random_device oRandomDevice;
	std::uniform_real_distribution<float> oUniformRandom(a_fMin, a_fMax);

	return oUniformRandom(oRandomDevice);
}

void RunEffectLoop(LPD3DXEFFECT a_pEffect, const std::string & a_rTechnique, const std::function<void(int)>& a_rCallback)
{
	a_pEffect->SetTechnique(a_rTechnique.c_str());

	UINT nNumPasses = 0;
	a_pEffect->Begin(&nNumPasses, 0);

	for (int i = 0; i < nNumPasses; ++i) {
		a_pEffect->BeginPass(i);

		a_rCallback(i);
		a_pEffect->EndPass();
	}

	a_pEffect->End();
}

STRay CreateRay(const POINT & a_rstPoint)
{
	D3DVIEWPORT9 stViewport;
	GET_DEVICE()->GetViewport(&stViewport);

	float fNormalizeX = ((a_rstPoint.x * 2.0f) / stViewport.Width) - 1.0f;
	float fNormalizeY = ((a_rstPoint.y * -2.0f) / stViewport.Height) + 1.0f;
	// }

	// 투영 -> 뷰 공간으로 좌표 이동
	// {
	D3DXMATRIXA16 stProjectionMatrix;
	GET_DEVICE()->GetTransform(D3DTS_PROJECTION, &stProjectionMatrix);

	STRay stRay;
	ZeroMemory(&stRay, sizeof(stRay));

	stRay.m_stDirection = D3DXVECTOR3(fNormalizeX / stProjectionMatrix(0, 0),
		fNormalizeY / stProjectionMatrix(1, 1),
		1.0f);
	// }

	// 뷰 -> 월드 공간으로 좌표 이동
	// {
	D3DXMATRIXA16 stViewMatrix;
	GET_DEVICE()->GetTransform(D3DTS_VIEW, &stViewMatrix);

	D3DXMATRIXA16 stInverseMatrix;
	D3DXMatrixInverse(&stInverseMatrix, NULL, &stViewMatrix);

	D3DXVec3TransformCoord(&stRay.m_stOrigin, &stRay.m_stOrigin, &stInverseMatrix);
	D3DXVec3TransformNormal(&stRay.m_stDirection, &stRay.m_stDirection, &stInverseMatrix);
	// }

	D3DXVec3Normalize(&stRay.m_stDirection, &stRay.m_stDirection);
	return stRay;
}

LPD3DXMESH CreateMesh(int a_nFaceSize, int a_nVertexSize, DWORD a_nOptions, DWORD a_nFVF)
{
	return LPD3DXMESH();
}

LPD3DXMESH CreateMesh(int a_nFaceSize, int a_nVertexSize, DWORD a_nOptions, D3DVERTEXELEMENT9 * a_pstElements)
{
	return LPD3DXMESH();
}

LPDIRECT3DVERTEXBUFFER9 CreateVertexBuffer(int a_nSize, DWORD a_nOptions, DWORD a_nFVF)
{
	return LPDIRECT3DVERTEXBUFFER9();
}

LPDIRECT3DINDEXBUFFER9 CreateIndexBuffer(int a_nSize, DWORD a_nOptions, D3DFORMAT a_nFormat)
{
	return LPDIRECT3DINDEXBUFFER9();
}

BOOL IsCreshWithBoundingSphere(STRay a_stRay, STBoundingSphere a_stBoundingSphere)
{
	D3DXVECTOR3 stDelta = a_stBoundingSphere.m_stPosition - a_stRay.m_stOrigin;
	D3DXVECTOR3 stDirection = a_stRay.m_stDirection;

	// 방향 벡터를 정규화한다
	D3DXVec3Normalize(&stDirection, &stDirection);

	float fDeltaLength = D3DXVec3Dot(&stDelta, &stDelta);
	float fRadius = powf(a_stBoundingSphere.m_fRadius, 2.0f);
	float fDotValue = D3DXVec3Dot(&stDelta, &stDirection);

	// 충돌이 발생하지 않았을 경우
	if (fDotValue < 0.0f && fDeltaLength > fRadius) {
		return false;
	}

	return fDeltaLength - (fDotValue * fDotValue) <= fRadius;
}
