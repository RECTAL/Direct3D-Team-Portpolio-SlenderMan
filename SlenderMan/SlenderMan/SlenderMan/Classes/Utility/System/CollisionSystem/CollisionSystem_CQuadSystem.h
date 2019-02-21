#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../Object/TerrainObject/CTerrainObject.h"


class CFrustum;
class CQuadTree
{
	enum CornerType {CORNER_TL,CORNER_TR,CORNER_BL,CORNER_BR};
	enum QuadLocation{FRUSTUM_OUT=0,FRUSTUM_PARTIALLY_IN=1,FRUSTUM_COMPLETELY_IN=2,FRUSTUM_UNKNOWN=-1};
public:
	CQuadTree(int cx, int cy);
	CQuadTree(CQuadTree* pParent);
	~CQuadTree();


public:
	BOOL	build(D3DXVECTOR3* pHeightMap);
	int		GenerateIndex(LPVOID pIB);
	int		GenerateIndex(LPVOID pIndex, D3DXVECTOR3* pHeightMap, CFrustum* pFrustum);

private:
	CQuadTree* addChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	BOOL	setCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	BOOL	subDivide();
	BOOL	isVisible() { return m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1; }

	int		genTriIndex(int nTriangles, LPVOID pIndex);
	void	destroy();

	int		isInFrustum(D3DXVECTOR3* pHeightMap, CFrustum* pFrustum);
	void	frustumCull(D3DXVECTOR3* pHeightMap, CFrustum* pFrustum);



private:
	CQuadTree * m_pChild[4] = {nullptr,};
	int				m_nCenter;
	int				m_nCorner[4];


	BOOL			m_bCulled;
	float			m_fRadius;
};