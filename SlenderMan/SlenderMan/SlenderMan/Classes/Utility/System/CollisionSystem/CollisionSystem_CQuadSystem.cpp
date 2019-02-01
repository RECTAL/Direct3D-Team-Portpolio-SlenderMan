#include "CollisionSystem_CQuadSystem.h"
#include "../../../Utility/System/CollisionSystem/CollisionSystem_CFrustum.h"

CQuadTree::CQuadTree(int cx, int cy)
{
	int i;
	m_nCenter = 0;
	for (int i = 0; i < 4; i++)
	{
		m_pChild[i] = NULL;
	}

	m_nCorner[CORNER_TL] = 0;
	m_nCorner[CORNER_TR] = cx - 1;
	m_nCorner[CORNER_BL] = cx * (cy - 1);
	m_nCorner[CORNER_BR] = cx * cy - 1;

	m_nCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;

	m_fRadius = 0.0f;
	m_bCulled - FALSE;
	m_fRadius = 0.0f;
}

CQuadTree::CQuadTree(CQuadTree * pParent)
{
	int i;
	m_nCenter = 0;
	for (i = 0; i < 4; i++) 
	{
		m_pChild[i] = NULL;
		m_nCorner[i] = 0;
	}
	m_bCulled = FALSE;
	m_fRadius = 0.0f;
}

CQuadTree::~CQuadTree()
{
	destroy();
}

BOOL CQuadTree::build(D3DXVECTOR3* pHeightMap)
{
	if (subDivide())
	{
		D3DXVECTOR3 v = *(pHeightMap + m_nCorner[CORNER_TL]) -
			*(pHeightMap + m_nCorner[CORNER_BR]);

		m_fRadius = D3DXVec3Length(&v) / 2.0f;

		m_pChild[CORNER_TL]->build(pHeightMap);
		m_pChild[CORNER_TR]->build(pHeightMap);
		m_pChild[CORNER_BL]->build(pHeightMap);
		m_pChild[CORNER_BR]->build(pHeightMap);
	}
	return TRUE;
}

int CQuadTree::GenerateIndex(LPVOID pIB)
{
	return genTriIndex(0, pIB);
}

int CQuadTree::GenerateIndex(LPVOID pIndex, D3DXVECTOR3 * pHeightMap, CFrustum * pFrustum)
{
	frustumCull(pHeightMap, pFrustum);
	return genTriIndex(0,pIndex);
}

CQuadTree * CQuadTree::addChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
{
	CQuadTree* pChild = nullptr;
	pChild = new CQuadTree(this);
	pChild->setCorners(nCornerTL, nCornerTR, nCornerBL, nCornerBR);

	return pChild;
}

BOOL CQuadTree::setCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR)
{
	m_nCorner[CORNER_TL] = nCornerTL;
	m_nCorner[CORNER_TR] = nCornerTR;
	m_nCorner[CORNER_BL] = nCornerBL;
	m_nCorner[CORNER_BR] = nCornerBR;
	
	m_nCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;

	return TRUE;
}

BOOL CQuadTree::subDivide()
{
	int nTopEdgeCenter;
	int nBottomEdgeCenter;
	int nLeftEdgeCenter;
	int nRightEdgeCenter;
	int nCentralPoint;

	nTopEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR]) / 2;
	nBottomEdgeCenter = (m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 2;
	nLeftEdgeCenter = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL]) / 2;
	nRightEdgeCenter = (m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR]) / 2;

	nCentralPoint = (m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR]) / 4;


	if ((m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1)
	{
		return FALSE;
	}

	m_pChild[CORNER_TL] = addChild(m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint);

	m_pChild[CORNER_TR] = addChild(nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter);

	m_pChild[CORNER_BL] = addChild(nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter);

	m_pChild[CORNER_BR] = addChild(nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR]);

	return TRUE;

}

int CQuadTree::genTriIndex(int nTriangles, LPVOID pIndex)
{
	if (m_bCulled)
	{
		m_bCulled = FALSE;
		return nTriangles;
	}

	if (isVisible())
	{
		LPDWORD p = ((LPDWORD)pIndex) + nTriangles * 3;
		*p++ = m_nCorner[0];
		*p++ = m_nCorner[1];
		*p++ = m_nCorner[2];
		nTriangles++;

		*p++ = m_nCorner[2];
		*p++ = m_nCorner[1];
		*p++ = m_nCorner[3];

		nTriangles++;
		return nTriangles;
	}

	if (m_pChild[CORNER_TL]) nTriangles = m_pChild[CORNER_TL]->genTriIndex(nTriangles, pIndex);

	if (m_pChild[CORNER_TR]) nTriangles = m_pChild[CORNER_TR]->genTriIndex(nTriangles, pIndex);

	if (m_pChild[CORNER_BL]) nTriangles = m_pChild[CORNER_BL]->genTriIndex(nTriangles, pIndex);

	if (m_pChild[CORNER_BR]) nTriangles = m_pChild[CORNER_BR]->genTriIndex(nTriangles, pIndex);

	return nTriangles;
}

void CQuadTree::destroy()
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(m_pChild[i]);
	}
}

int CQuadTree::isInFrustum(D3DXVECTOR3* pHeightMap, CFrustum * pFrustum)
{
	BOOL b[4];
	BOOL bInSphere;

	bInSphere = pFrustum->IsInSphere((D3DXVECTOR3*)(pHeightMap + m_nCenter), m_fRadius);
	if (!bInSphere)return FRUSTUM_OUT;

	b[0] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[0]));
	b[1] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[1]));
	b[2] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[2]));
	b[3] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_nCorner[3]));

	if ((b[0] + b[1] + b[2] + b[3]) == 4)return FRUSTUM_COMPLETELY_IN;

	return FRUSTUM_PARTIALLY_IN;
}

void CQuadTree::frustumCull(D3DXVECTOR3 * pHeightMap, CFrustum * pFrustum)
{
	int ret;

	ret = isInFrustum(pHeightMap, pFrustum);
	switch (ret)
	{
	case FRUSTUM_COMPLETELY_IN:
		m_bCulled = FALSE;
		return;
	case FRUSTUM_PARTIALLY_IN:
		m_bCulled = FALSE;
		break;
	case FRUSTUM_OUT:
		m_bCulled = TRUE;
		return;
	}
	if (m_pChild[0])m_pChild[0]->frustumCull(pHeightMap, pFrustum);
	if (m_pChild[1])m_pChild[1]->frustumCull(pHeightMap, pFrustum);
	if (m_pChild[2])m_pChild[2]->frustumCull(pHeightMap, pFrustum);
	if (m_pChild[3])m_pChild[3]->frustumCull(pHeightMap, pFrustum);
}
