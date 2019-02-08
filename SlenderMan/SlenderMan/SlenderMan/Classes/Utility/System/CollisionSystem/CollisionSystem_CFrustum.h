#pragma once
#include "../../../Define/KGlobalDefine.h"


#define PLANE_EPSILON	5.0f

class CFrustum
{
	D3DXVECTOR3	m_vtx[8];	
	D3DXVECTOR3	m_vPos;		
	D3DXPLANE	m_plane[6];

public:
	/// 생성자
	CFrustum();

	/// 카메라(view) * 프로젝션(projection)행렬을 입력받아 6개의 평면을 만든다.
	BOOL	Make(D3DXMATRIXA16* pmatViewProj);

	/// 한점 v가 프러스텀안에 있으면 TRUE를 반환, 아니면 FALSE를 반환한다.
	BOOL	IsIn(D3DXVECTOR3* pv);

	/** 중심(v)와 반지름(radius)를 갖는 경계구(bounding sphere)가 프러스텀안에 있으면
	 *  TRUE를 반환, 아니면 FALSE를 반환한다.
	 */
	BOOL	IsInSphere(D3DXVECTOR3* pv, float radius);
	BOOL	IsInSphere(STBoundingSphere a_stBoundingSphere);

	/// 프러스텀을 화면에 그려준다.
	BOOL	Draw(LPDIRECT3DDEVICE9 pDev);

	/// 현재 카메라의 월드좌표를 얻어준다.
	D3DXVECTOR3*	GetPos() { return &m_vPos; }
};

/// 평면의 방정식을 직접구하는 함수, 현재는 사용되고 있지 않다.
void ZMakePlane(D3DXPLANE* pPlane, D3DXVECTOR3* pv0, D3DXVECTOR3* pv1, D3DXVECTOR3* pv2);
