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

STBoundingBox ConvertToBoundingBox(const STObjectBox & a_rstObjectBox)
{
	STBoundingBox stBoundingBox;
	ZeroMemory(&stBoundingBox, sizeof(stBoundingBox));

	auto stCenter = a_rstObjectBox.m_stCenter;
	auto stRightDirection = a_rstObjectBox.m_astDirections[0] * a_rstObjectBox.m_fHalfLengths[0];
	auto stUpDirection = a_rstObjectBox.m_astDirections[1] * a_rstObjectBox.m_fHalfLengths[1];
	auto stForwardDirection = a_rstObjectBox.m_astDirections[2] * a_rstObjectBox.m_fHalfLengths[2];

	// 경계 볼륨을 계산한다
	// {
	D3DXVECTOR3 astVertices[] = {
		stCenter + stRightDirection + stUpDirection + stForwardDirection,
		stCenter + stRightDirection - stUpDirection + stForwardDirection,
		stCenter - stRightDirection + stUpDirection + stForwardDirection,
		stCenter - stRightDirection - stUpDirection + stForwardDirection,

		stCenter + stRightDirection + stUpDirection - stForwardDirection,
		stCenter + stRightDirection - stUpDirection - stForwardDirection,
		stCenter - stRightDirection + stUpDirection - stForwardDirection,
		stCenter - stRightDirection - stUpDirection - stForwardDirection
	};

	stBoundingBox.m_stMinPoint = stCenter;
	stBoundingBox.m_stMaxPoint = stCenter;

	for (int i = 0; i < sizeof(astVertices) / sizeof(astVertices[0]); ++i) {
		D3DXVECTOR3 stVertex = astVertices[i];

		// X 위치 계산
		// {
		if (stVertex.x < stBoundingBox.m_stMinPoint.x) {
			stBoundingBox.m_stMinPoint.x = stVertex.x;
		}

		if (stVertex.x >= stBoundingBox.m_stMaxPoint.x) {
			stBoundingBox.m_stMaxPoint.x = stVertex.x;
		}
		// }

		// Y 위치 계산
		// {
		if (stVertex.y < stBoundingBox.m_stMinPoint.y) {
			stBoundingBox.m_stMinPoint.y = stVertex.y;
		}

		if (stVertex.y >= stBoundingBox.m_stMaxPoint.y) {
			stBoundingBox.m_stMaxPoint.y = stVertex.y;
		}
		// }

		// Z 위치 계산
		// {
		if (stVertex.z < stBoundingBox.m_stMinPoint.z) {
			stBoundingBox.m_stMinPoint.z = stVertex.z;
		}

		if (stVertex.z >= stBoundingBox.m_stMaxPoint.z) {
			stBoundingBox.m_stMaxPoint.z = stVertex.z;
		}
		// }
	}
	// }

	return stBoundingBox;
}

bool IsIntersectBox(const STBoundingBox & a_rstLhs, const STBoundingBox & a_rstRhs)
{
	if (a_rstLhs.m_stMinPoint.x > a_rstRhs.m_stMaxPoint.x ||
		a_rstLhs.m_stMaxPoint.x < a_rstRhs.m_stMinPoint.x)
	{
		return false;
	}

	if (a_rstLhs.m_stMinPoint.y > a_rstRhs.m_stMaxPoint.y ||
		a_rstLhs.m_stMaxPoint.y < a_rstRhs.m_stMinPoint.y)
	{
		return false;
	}

	if (a_rstLhs.m_stMinPoint.z > a_rstRhs.m_stMaxPoint.z ||
		a_rstLhs.m_stMaxPoint.z < a_rstRhs.m_stMinPoint.z)
	{
		return false;
	}

	return true;
}

bool IsIntersectObjectBox(const STObjectBox & a_rstLhs, const STObjectBox & a_rstRhs)
{
	float fCutOffValue = 0.99999f;
	bool bIsExistParallel = false;

	float afCosValues[3][3] = { 0.0f };
	float afAbsCosValues[3][3] = { 0.0f };

	float afDeltaDotValues[3] = { 0.0f };
	D3DXVECTOR3 stDelta = a_rstRhs.m_stCenter - a_rstLhs.m_stCenter;

	float fDotValueA = 0.0f;
	float fDotValueB = 0.0f;
	float fTotalDotValue = 0.0f;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			afCosValues[i][j] = D3DXVec3Dot(&a_rstLhs.m_astDirections[i],
				&a_rstRhs.m_astDirections[j]);

			afAbsCosValues[i][j] = fabsf(afCosValues[i][j]);

			// 평행 한 축이 존재 할 경우
			if (afAbsCosValues[i][j] > fCutOffValue) {
				bIsExistParallel = true;
			}
		}

		afDeltaDotValues[i] = D3DXVec3Dot(&a_rstLhs.m_astDirections[i], &stDelta);
	}

	// Lhs 를 기준으로 충돌 검출
	// {
	// X 축 검사
	fTotalDotValue = fabsf(afDeltaDotValues[0]);
	fDotValueA = a_rstLhs.m_fHalfLengths[0];

	fDotValueB = (afAbsCosValues[0][0] * a_rstRhs.m_fHalfLengths[0]) +
		(afAbsCosValues[0][1] * a_rstRhs.m_fHalfLengths[1]) +
		(afAbsCosValues[0][2] * a_rstRhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Y 축 검사
	fTotalDotValue = fabsf(afDeltaDotValues[1]);
	fDotValueA = a_rstLhs.m_fHalfLengths[1];

	fDotValueB = (afAbsCosValues[1][0] * a_rstRhs.m_fHalfLengths[0]) +
		(afAbsCosValues[1][1] * a_rstRhs.m_fHalfLengths[1]) +
		(afAbsCosValues[1][2] * a_rstRhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Z 축 검사
	fTotalDotValue = fabsf(afDeltaDotValues[2]);
	fDotValueA = a_rstLhs.m_fHalfLengths[2];

	fDotValueB = (afAbsCosValues[2][0] * a_rstRhs.m_fHalfLengths[0]) +
		(afAbsCosValues[2][1] * a_rstRhs.m_fHalfLengths[1]) +
		(afAbsCosValues[2][2] * a_rstRhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}
	// }

	// Rhs 를 기준으로 충돌 검출
	// {
	// X 축 검사
	fTotalDotValue = fabsf(D3DXVec3Dot(&a_rstRhs.m_astDirections[0], &stDelta));
	fDotValueB = a_rstRhs.m_fHalfLengths[0];

	fDotValueA = (afAbsCosValues[0][0] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[1][0] * a_rstLhs.m_fHalfLengths[1]) +
		(afAbsCosValues[2][0] * a_rstLhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Y 축 검사
	fTotalDotValue = fabsf(D3DXVec3Dot(&a_rstRhs.m_astDirections[1], &stDelta));
	fDotValueB = a_rstRhs.m_fHalfLengths[1];

	fDotValueA = (afAbsCosValues[0][1] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[1][1] * a_rstLhs.m_fHalfLengths[1]) +
		(afAbsCosValues[2][1] * a_rstLhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Z 축 검사
	fTotalDotValue = fabsf(D3DXVec3Dot(&a_rstRhs.m_astDirections[2], &stDelta));
	fDotValueB = a_rstRhs.m_fHalfLengths[2];

	fDotValueA = (afAbsCosValues[0][2] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[1][2] * a_rstLhs.m_fHalfLengths[1]) +
		(afAbsCosValues[2][2] * a_rstLhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}
	// }

	// 평행 한 축이 존재 할 경우
	if (bIsExistParallel) {
		return true;
	}

	// Lhs X Rhs 를 기준으로 충돌 검출
	// {
	// X x X 축 검사
	fTotalDotValue = fabsf((afDeltaDotValues[2] * afCosValues[1][0]) -
		(afDeltaDotValues[1] * afCosValues[2][0]));

	fDotValueA = (afAbsCosValues[2][0] * a_rstLhs.m_fHalfLengths[1]) +
		(afAbsCosValues[1][0] * a_rstLhs.m_fHalfLengths[2]);

	fDotValueB = (afAbsCosValues[0][2] * a_rstLhs.m_fHalfLengths[1]) +
		(afAbsCosValues[0][1] * a_rstLhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// X x Y 축 검사
	fTotalDotValue = fabsf((afDeltaDotValues[2] * afCosValues[1][1]) -
		(afDeltaDotValues[1] * afCosValues[2][1]));

	fDotValueA = (afAbsCosValues[2][1] * a_rstLhs.m_fHalfLengths[1]) +
		(afAbsCosValues[1][1] * a_rstLhs.m_fHalfLengths[2]);

	fDotValueB = (afAbsCosValues[0][2] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[0][0] * a_rstLhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// X x Z 축 검사
	fTotalDotValue = fabsf((afDeltaDotValues[2] * afCosValues[1][2]) -
		(afDeltaDotValues[1] * afCosValues[2][2]));

	fDotValueA = (afAbsCosValues[2][2] * a_rstLhs.m_fHalfLengths[1]) +
		(afAbsCosValues[1][2] * a_rstLhs.m_fHalfLengths[2]);

	fDotValueB = (afAbsCosValues[0][1] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[0][0] * a_rstLhs.m_fHalfLengths[1]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Y x X 축 검사
	fTotalDotValue = fabsf((afDeltaDotValues[0] * afCosValues[2][0]) -
		(afDeltaDotValues[2] * afCosValues[0][0]));

	fDotValueA = (afAbsCosValues[2][0] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[0][0] * a_rstLhs.m_fHalfLengths[2]);

	fDotValueB = (afAbsCosValues[1][2] * a_rstLhs.m_fHalfLengths[1]) +
		(afAbsCosValues[1][1] * a_rstLhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Y x Y 축 검사
	fTotalDotValue = fabsf((afDeltaDotValues[0] * afCosValues[2][1]) -
		(afDeltaDotValues[2] * afCosValues[0][1]));

	fDotValueA = (afAbsCosValues[2][1] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[0][1] * a_rstLhs.m_fHalfLengths[2]);

	fDotValueB = (afAbsCosValues[1][2] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[1][0] * a_rstLhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Y x Z 축 검사
	fTotalDotValue = fabsf((afDeltaDotValues[0] * afCosValues[2][2]) -
		(afDeltaDotValues[2] * afCosValues[0][2]));

	fDotValueA = (afAbsCosValues[2][2] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[0][2] * a_rstLhs.m_fHalfLengths[2]);

	fDotValueB = (afAbsCosValues[1][1] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[1][0] * a_rstLhs.m_fHalfLengths[1]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Z x X 축 검사
	fTotalDotValue = fabsf((afDeltaDotValues[1] * afCosValues[0][0]) -
		(afDeltaDotValues[0] * afCosValues[1][0]));

	fDotValueA = (afAbsCosValues[1][0] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[0][0] * a_rstLhs.m_fHalfLengths[1]);

	fDotValueB = (afAbsCosValues[2][2] * a_rstLhs.m_fHalfLengths[1]) +
		(afAbsCosValues[2][1] * a_rstLhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Z x Y 축 검사
	fTotalDotValue = fabsf((afDeltaDotValues[1] * afCosValues[0][1]) -
		(afDeltaDotValues[0] * afCosValues[1][1]));

	fDotValueA = (afAbsCosValues[1][1] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[0][1] * a_rstLhs.m_fHalfLengths[1]);

	fDotValueB = (afAbsCosValues[2][2] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[2][0] * a_rstLhs.m_fHalfLengths[2]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}

	// Z x Z 축 검사
	fTotalDotValue = fabsf((afDeltaDotValues[1] * afCosValues[0][2]) -
		(afDeltaDotValues[0] * afCosValues[1][2]));

	fDotValueA = (afAbsCosValues[1][2] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[0][2] * a_rstLhs.m_fHalfLengths[1]);

	fDotValueB = (afAbsCosValues[2][1] * a_rstLhs.m_fHalfLengths[0]) +
		(afAbsCosValues[2][0] * a_rstLhs.m_fHalfLengths[1]);

	if (fTotalDotValue > fDotValueA + fDotValueB) {
		return false;
	}
	// }

	return true;
}

bool IsIntersectSphere(const STBoundingSphere & a_rstLhs, const STBoundingSphere & a_rstRhs)
{
	auto stDistance = a_rstLhs.m_stCenter - a_rstRhs.m_stCenter;
	return D3DXVec3Length(&stDistance) <= a_rstLhs.m_fRadius + a_rstRhs.m_fRadius;
}

bool IsIntersectRayBox(const STRay & a_rstRay, const STBoundingBox & a_rstBoundingBox)
{
	float fMinMaxValue = -FLT_MAX;
	float fMaxMinValue = FLT_MAX;

	// X 축 검사
	// {
	if (fabsf(a_rstRay.m_stDirection.x) <= FLT_EPSILON) {
		if (a_rstRay.m_stOrigin.x < a_rstBoundingBox.m_stMinPoint.x ||
			a_rstRay.m_stOrigin.x > a_rstBoundingBox.m_stMaxPoint.x)
		{
			return false;
		}
	}
	else {
		float fMinValue = (a_rstBoundingBox.m_stMinPoint.x - a_rstRay.m_stOrigin.x) / a_rstRay.m_stDirection.x;
		float fMaxValue = (a_rstBoundingBox.m_stMaxPoint.x - a_rstRay.m_stOrigin.x) / a_rstRay.m_stDirection.x;

		if (fMinValue > fMaxValue) {
			std::swap(fMinValue, fMaxValue);
		}

		if (fMinMaxValue < fMinValue) {
			fMinMaxValue = fMinValue;
		}

		if (fMaxMinValue > fMaxValue) {
			fMaxMinValue = fMaxValue;
		}

		if (fMaxMinValue < 0 || fMinMaxValue > fMaxMinValue) {
			return false;
		}
	}
	// }

	// Y 축 검사
	// {
	if (fabsf(a_rstRay.m_stDirection.y) <= FLT_EPSILON) {
		if (a_rstRay.m_stOrigin.y < a_rstBoundingBox.m_stMinPoint.y ||
			a_rstRay.m_stOrigin.y > a_rstBoundingBox.m_stMaxPoint.y)
		{
			return false;
		}
	}
	else {
		float fMinValue = (a_rstBoundingBox.m_stMinPoint.y - a_rstRay.m_stOrigin.y) / a_rstRay.m_stDirection.y;
		float fMaxValue = (a_rstBoundingBox.m_stMaxPoint.y - a_rstRay.m_stOrigin.y) / a_rstRay.m_stDirection.y;

		if (fMinValue > fMaxValue) {
			std::swap(fMinValue, fMaxValue);
		}

		if (fMinMaxValue < fMinValue) {
			fMinMaxValue = fMinValue;
		}

		if (fMaxMinValue > fMaxValue) {
			fMaxMinValue = fMaxValue;
		}

		if (fMaxMinValue < 0 || fMinMaxValue > fMaxMinValue) {
			return false;
		}
	}
	// }

	// Z 축 검사
	// {
	if (fabsf(a_rstRay.m_stDirection.z) <= FLT_EPSILON) {
		if (a_rstRay.m_stOrigin.z < a_rstBoundingBox.m_stMinPoint.z ||
			a_rstRay.m_stOrigin.z > a_rstBoundingBox.m_stMaxPoint.z)
		{
			return false;
		}
	}
	else {
		float fMinValue = (a_rstBoundingBox.m_stMinPoint.z - a_rstRay.m_stOrigin.z) / a_rstRay.m_stDirection.z;
		float fMaxValue = (a_rstBoundingBox.m_stMaxPoint.z - a_rstRay.m_stOrigin.z) / a_rstRay.m_stDirection.z;

		if (fMinValue > fMaxValue) {
			std::swap(fMinValue, fMaxValue);
		}

		if (fMinMaxValue < fMinValue) {
			fMinMaxValue = fMinValue;
		}

		if (fMaxMinValue > fMaxValue) {
			fMaxMinValue = fMaxValue;
		}

		if (fMaxMinValue < 0 || fMinMaxValue > fMaxMinValue) {
			return false;
		}
	}
	// }

	return true;
}

bool IsIntersectRaySphere(const STRay & a_rstRay, const STBoundingSphere & a_rstBoundingSphere)
{
	D3DXVECTOR3 stDelta = a_rstBoundingSphere.m_stCenter - a_rstRay.m_stOrigin;
	D3DXVECTOR3 stDirection = a_rstRay.m_stDirection;

	// 방향 벡터를 정규화한다
	D3DXVec3Normalize(&stDirection, &stDirection);

	float fDeltaLength = D3DXVec3Dot(&stDelta, &stDelta);
	float fRadius = powf(a_rstBoundingSphere.m_fRadius, 2.0f);
	float fDotValue = D3DXVec3Dot(&stDelta, &stDirection);

	// 충돌이 발생하지 않았을 경우
	if (fDotValue < 0.0f && fDeltaLength > fRadius) {
		return false;
	}

	return fDeltaLength - (fDotValue * fDotValue) <= fRadius;
}

STBoundingBox CreateBoundingBox(LPD3DXMESH a_pMesh)
{
	STBoundingBox stBoundingBox;
	ZeroMemory(&stBoundingBox, sizeof(stBoundingBox));

	// 경계 볼륨을 설정한다
	// {
	void *pvVertices = nullptr;

	/*
	D3DXComputeBoundingBox, D3DXComputeBoundingSphere 함수를 이용하면
	내부적으로 정점의 위치를 참조해서 각각 경계 볼륨과 경계 구 정보를 계산하는
	것이 가능하다. (단, 해당 함수는 정적인 정보만을 이용해서 데이터를 생성하는
	것이기 때문에 동적으로 동작하는 물체에서는 적절한 데이터의 보정이 필요하다.)
	*/
	if (SUCCEEDED(a_pMesh->LockVertexBuffer(0, &pvVertices))) {
		// 경계 볼륨을 계산한다
		D3DXComputeBoundingBox((D3DXVECTOR3 *)pvVertices,
			a_pMesh->GetNumVertices(),
			a_pMesh->GetNumBytesPerVertex(),
			&stBoundingBox.m_stMinPoint,
			&stBoundingBox.m_stMaxPoint);

		a_pMesh->UnlockVertexBuffer();
	}
	// }

	return stBoundingBox;
}

STBoundingSphere CreateBoundingSphere(LPD3DXMESH a_pMesh)
{
	STBoundingSphere stBoundingSphere;
	ZeroMemory(&stBoundingSphere, sizeof(stBoundingSphere));

	// 구 정보를 설정한다
	// {
	void *pvVertices = nullptr;

	if (SUCCEEDED(a_pMesh->LockVertexBuffer(0, &pvVertices))) {
		// 경계 구를 계산한다
		D3DXComputeBoundingSphere((D3DXVECTOR3 *)pvVertices,
			a_pMesh->GetNumVertices(),
			a_pMesh->GetNumBytesPerVertex(),
			&stBoundingSphere.m_stCenter,
			&stBoundingSphere.m_fRadius);

		a_pMesh->UnlockVertexBuffer();
	}
	// }

	return stBoundingSphere;
}
