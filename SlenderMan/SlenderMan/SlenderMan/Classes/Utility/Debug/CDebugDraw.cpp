#include "CDebugDraw.h"
#include "../../Function/GlobalFunction.h"
#include "../Manager/CDeviceManager.h"

CDebugDraw::CDebugDraw(CRenderObject * a_pRenderObject, EDebugDrawType a_eDebugDrawType,BOOL a_bIsDraw)
	:
	m_pRenderObject(a_pRenderObject),
	m_eDebugDrawType(a_eDebugDrawType)
{
	ZeroMemory(&m_stBoundingBox, sizeof(m_stBoundingBox));
	ZeroMemory(&m_stBoundingSphere, sizeof(m_stBoundingSphere));

	
	m_stBoundingBox = m_pRenderObject->getBoundingBox();
	m_pBoundingBox = this->createBoundingBox(m_stBoundingBox);
	
	
	m_stBoundingSphere = m_pRenderObject->getBoundingSphere();
	m_pBoundingSphere = this->createBoundingSphere(m_stBoundingSphere);

	m_bIsDraw = a_bIsDraw;
	
}

CDebugDraw::~CDebugDraw(void)
{
	SAFE_RELEASE(m_pBoundingBox);
	SAFE_RELEASE(m_pBoundingSphere);
}

void CDebugDraw::preDraw(void)
{
	CRenderObject::preDraw();

	GET_DEVICE()->SetRenderState(D3DRS_LIGHTING, FALSE);
	GET_DEVICE()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	GET_DEVICE()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

void CDebugDraw::doDraw(void)
{
	CRenderObject::doDraw();
	if (m_bIsDraw)
	{
		if (m_eDebugDrawType == EDebugDrawType::BOX) {
			this->drawBoundingBox();
		}
		else {
			this->drawBoundingSphere();
		}

		this->drawGuideLine();
	}
}

void CDebugDraw::postDraw(void)
{
	CRenderObject::postDraw();

	GET_DEVICE()->SetRenderState(D3DRS_LIGHTING, TRUE);
	GET_DEVICE()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GET_DEVICE()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CDebugDraw::drawBoundingBox(void)
{
	// 행렬을 설정한다
	// {
	D3DXMATRIXA16 stTranslate;

	D3DXMatrixTranslation(&stTranslate,
		(m_stBoundingBox.m_stMinPoint.x + m_stBoundingBox.m_stMaxPoint.x) / 2.0f,
		(m_stBoundingBox.m_stMinPoint.y + m_stBoundingBox.m_stMaxPoint.y) / 2.0f,
		(m_stBoundingBox.m_stMinPoint.z + m_stBoundingBox.m_stMaxPoint.z) / 2.0f);

	D3DXMATRIXA16 stWorldMatrix = stTranslate * m_pRenderObject->getFinalWorldMatrix();
	GET_DEVICE()->SetTransform(D3DTS_WORLD, &stWorldMatrix);
	// }

	m_pBoundingBox->DrawSubset(0);
}

void CDebugDraw::drawBoundingSphere(void)
{
	// 행렬을 설정한다
	// {
	D3DXMATRIXA16 stTranslate;

	D3DXMatrixTranslation(&stTranslate,
		m_stBoundingSphere.m_stCenter.x,
		m_stBoundingSphere.m_stCenter.y,
		m_stBoundingSphere.m_stCenter.z);

	D3DXMATRIXA16 stWorldMatrix = stTranslate * m_pRenderObject->getFinalWorldMatrix();
	GET_DEVICE()->SetTransform(D3DTS_WORLD, &stWorldMatrix);
	// }

	m_pBoundingSphere->DrawSubset(0);
}

void CDebugDraw::drawGuideLine(void)
{
	STVertex astVertices[2];
	ZeroMemory(astVertices, sizeof(astVertices));

	astVertices[0].m_nDiffuse = D3DCOLOR_XRGB(255, 255, 0);
	astVertices[1].m_nDiffuse = D3DCOLOR_XRGB(255, 255, 0);

	// 행렬을 설정한다
	// {
	D3DXMATRIXA16 stWorldMatrix;
	D3DXMatrixIdentity(&stWorldMatrix);

	GET_DEVICE()->SetTransform(D3DTS_WORLD, &stWorldMatrix);
	// }

	if (m_eDebugDrawType == EDebugDrawType::BOX) {
		auto stBoundingBox = m_pRenderObject->getFinalBoundingBox();

		astVertices[0].m_stPosition = stBoundingBox.m_stMinPoint;
		astVertices[1].m_stPosition = stBoundingBox.m_stMaxPoint;

		this->drawBoundingBoxGuideLine();
	}
	else {
		auto stBoundingSphere = m_pRenderObject->getFinalBoundingSphere();

		astVertices[0].m_stPosition = stBoundingSphere.m_stCenter;
		astVertices[1].m_stPosition = stBoundingSphere.m_stCenter + (D3DXVECTOR3(0.707f, 0.707f, -0.707f) * stBoundingSphere.m_fRadius);
	}

	/*
	DrawPrimitiveUP, DrawIndexedPrimitiveUP 함수는 정점 버퍼를 지정하지 않고
	즉석에서 물체를 그리는 역할을 한다. (단, 정점 버퍼가 없이 물체를 그릴 수 있다는
	것은 내부적으로 정점에 대한 정보를 캐시하지 않기 때문에 정점 버퍼를 이용한 렌더링
	방식보다는 부하가 걸린다.)
	*/
	// 라인을 그린다
	GET_DEVICE()->SetFVF(STVertex::FVF);
	GET_DEVICE()->DrawPrimitiveUP(D3DPT_LINELIST, 1, astVertices, sizeof(STVertex));
}

void CDebugDraw::drawBoundingBoxGuideLine(void)
{
	STVertex astVertices[24];
	ZeroMemory(astVertices, sizeof(astVertices));

	for (int i = 0; i < 24; ++i) {
		astVertices[i].m_nDiffuse = D3DCOLOR_XRGB(0, 255, 0);
	}

	auto stObjectBox = m_pRenderObject->getObjectBox();
	auto stBoundingBox = ConvertToBoundingBox(stObjectBox);

	// 아랫면
	// {
	astVertices[0].m_stPosition = stBoundingBox.m_stMinPoint;
	astVertices[1].m_stPosition = astVertices[0].m_stPosition + D3DXVECTOR3(0.0f, 0.0f, stBoundingBox.m_stMaxPoint.z - stBoundingBox.m_stMinPoint.z);

	astVertices[2].m_stPosition = astVertices[1].m_stPosition;
	astVertices[3].m_stPosition = astVertices[2].m_stPosition + D3DXVECTOR3(stBoundingBox.m_stMaxPoint.x - stBoundingBox.m_stMinPoint.x, 0.0f, 0.0f);

	astVertices[4].m_stPosition = astVertices[3].m_stPosition;
	astVertices[5].m_stPosition = astVertices[4].m_stPosition + D3DXVECTOR3(0.0f, 0.0f, stBoundingBox.m_stMinPoint.z - stBoundingBox.m_stMaxPoint.z);

	astVertices[6].m_stPosition = astVertices[5].m_stPosition;
	astVertices[7].m_stPosition = astVertices[6].m_stPosition + D3DXVECTOR3(stBoundingBox.m_stMinPoint.x - stBoundingBox.m_stMaxPoint.x, 0.0f, 0.0f);
	// }

	// 윗면
	// {
	astVertices[0 + 8].m_stPosition = stBoundingBox.m_stMinPoint + D3DXVECTOR3(0.0f, stBoundingBox.m_stMaxPoint.y - stBoundingBox.m_stMinPoint.y, 0.0f);
	astVertices[1 + 8].m_stPosition = astVertices[0 + 8].m_stPosition + D3DXVECTOR3(0.0f, 0.0f, stBoundingBox.m_stMaxPoint.z - stBoundingBox.m_stMinPoint.z);

	astVertices[2 + 8].m_stPosition = astVertices[1 + 8].m_stPosition;
	astVertices[3 + 8].m_stPosition = astVertices[2 + 8].m_stPosition + D3DXVECTOR3(stBoundingBox.m_stMaxPoint.x - stBoundingBox.m_stMinPoint.x, 0.0f, 0.0f);

	astVertices[4 + 8].m_stPosition = astVertices[3 + 8].m_stPosition;
	astVertices[5 + 8].m_stPosition = astVertices[4 + 8].m_stPosition + D3DXVECTOR3(0.0f, 0.0f, stBoundingBox.m_stMinPoint.z - stBoundingBox.m_stMaxPoint.z);

	astVertices[6 + 8].m_stPosition = astVertices[5 + 8].m_stPosition;
	astVertices[7 + 8].m_stPosition = astVertices[6 + 8].m_stPosition + D3DXVECTOR3(stBoundingBox.m_stMinPoint.x - stBoundingBox.m_stMaxPoint.x, 0.0f, 0.0f);
	// }

	// 세로 선
	// {
	astVertices[16].m_stPosition = astVertices[0 + 0].m_stPosition;
	astVertices[17].m_stPosition = astVertices[8 + 0].m_stPosition;

	astVertices[18].m_stPosition = astVertices[0 + 2].m_stPosition;
	astVertices[19].m_stPosition = astVertices[8 + 2].m_stPosition;

	astVertices[20].m_stPosition = astVertices[0 + 4].m_stPosition;
	astVertices[21].m_stPosition = astVertices[8 + 4].m_stPosition;

	astVertices[22].m_stPosition = astVertices[0 + 6].m_stPosition;
	astVertices[23].m_stPosition = astVertices[8 + 6].m_stPosition;
	// }

	GET_DEVICE()->SetFVF(STVertex::FVF);
	GET_DEVICE()->DrawPrimitiveUP(D3DPT_LINELIST, 12, astVertices, sizeof(STVertex));
}

LPD3DXMESH CDebugDraw::createBoundingBox(const STBoundingBox & a_rstBoundingBox)
{
	LPD3DXMESH pMesh = nullptr;

	// 메시를 생성한다
	D3DXCreateBox(GET_DEVICE(),
		a_rstBoundingBox.m_stMaxPoint.x - a_rstBoundingBox.m_stMinPoint.x,
		a_rstBoundingBox.m_stMaxPoint.y - a_rstBoundingBox.m_stMinPoint.y,
		a_rstBoundingBox.m_stMaxPoint.z - a_rstBoundingBox.m_stMinPoint.z,
		&pMesh,
		NULL);

	return pMesh;
}

LPD3DXMESH CDebugDraw::createBoundingSphere(const STBoundingSphere & a_rstBoundingSphere)
{
	LPD3DXMESH pMesh = nullptr;

	// 메시를 생성한다
	D3DXCreateSphere(GET_DEVICE(),
		a_rstBoundingSphere.m_fRadius,
		10,
		10,
		&pMesh,
		NULL);

	return pMesh;
}
