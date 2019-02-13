#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../Base/CRenderObject.h"

//! ����� ��ü
class CDebugDraw : public CRenderObject
{
public:

	//! ����
	struct STVertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};

		D3DXVECTOR3 m_stPosition;
		D3DCOLOR m_nDiffuse;
	};

public:			// ������, �Ҹ���

	//! ������
	CDebugDraw(CRenderObject *a_pRenderObject,
		EDebugDrawType a_eDebugDrawType);

	//! �Ҹ���
	virtual ~CDebugDraw(void);

protected:			// protected �Լ�

	//! ��ü�� �׸��� ��
	virtual void preDraw(void) override;

	//! ��ü�� �׸���
	virtual void doDraw(void) override;

	//! ��ü�� �׸� ��
	virtual void postDraw(void) override;

private:			// private �Լ�

	//! ��� ������ �׸���
	void drawBoundingBox(void);

	//! ��� ���� �׸���
	void drawBoundingSphere(void);

	//! ���̵� ������ �׸���
	void drawGuideLine(void);

	//! ��� ���� ���̵� ������ �׸���
	void drawBoundingBoxGuideLine(void);

	//! ��� ������ �����Ѵ�
	LPD3DXMESH createBoundingBox(const STBoundingBox &a_rstBoundingBox);

	//! ��� ���� �����Ѵ�
	LPD3DXMESH createBoundingSphere(const STBoundingSphere &a_rstBoundingSphere);

private:			// private ����

	STBoundingBox m_stBoundingBox;
	STBoundingSphere m_stBoundingSphere;

	LPD3DXMESH m_pBoundingBox = nullptr;
	LPD3DXMESH m_pBoundingSphere = nullptr;

	CRenderObject *m_pRenderObject = nullptr;
	EDebugDrawType m_eDebugDrawType = EDebugDrawType::NONE;
};
