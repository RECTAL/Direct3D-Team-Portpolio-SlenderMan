#pragma once
#include "../../../Define/KGlobalDefine.h"


#define PLANE_EPSILON	5.0f

class CFrustum
{
	D3DXVECTOR3	m_vtx[8];	
	D3DXVECTOR3	m_vPos;		
	D3DXPLANE	m_plane[6];

public:
	/// ������
	CFrustum();

	/// ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
	BOOL	Make(D3DXMATRIXA16* pmatViewProj);

	/// ���� v�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	BOOL	IsIn(D3DXVECTOR3* pv);

	/** �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
	 *  TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	 */
	BOOL	IsInSphere(D3DXVECTOR3* pv, float radius);
	BOOL	IsInSphere(STBoundingSphere a_stBoundingSphere);

	/// ���������� ȭ�鿡 �׷��ش�.
	BOOL	Draw(LPDIRECT3DDEVICE9 pDev);

	/// ���� ī�޶��� ������ǥ�� ����ش�.
	D3DXVECTOR3*	GetPos() { return &m_vPos; }
};

/// ����� �������� �������ϴ� �Լ�, ����� ���ǰ� ���� �ʴ�.
void ZMakePlane(D3DXPLANE* pPlane, D3DXVECTOR3* pv0, D3DXVECTOR3* pv1, D3DXVECTOR3* pv2);
