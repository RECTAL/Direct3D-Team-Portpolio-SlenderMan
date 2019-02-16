#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../Base/CRenderObject.h"

//! 디버그 객체
class CDebugDraw : public CRenderObject
{
public:

	//! 정점
	struct STVertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};

		D3DXVECTOR3 m_stPosition;
		D3DCOLOR m_nDiffuse;
	};

public:			// 생성자, 소멸자

	//! 생성자
	CDebugDraw(CRenderObject *a_pRenderObject,
		EDebugDrawType a_eDebugDrawType,BOOL a_bIsDraw = true);

	//! 소멸자
	virtual ~CDebugDraw(void);

protected:			// protected 함수

	//! 물체를 그리기 전
	virtual void preDraw(void) override;

	//! 물체를 그린다
	virtual void doDraw(void) override;

	//! 물체를 그린 후
	virtual void postDraw(void) override;

private:			// private 함수

	//! 경계 볼륨을 그린다
	void drawBoundingBox(void);

	//! 경계 구를 그린다
	void drawBoundingSphere(void);

	//! 가이드 라인을 그린다
	void drawGuideLine(void);

	//! 경계 볼륨 가이드 라인을 그린다
	void drawBoundingBoxGuideLine(void);

	//! 경계 볼륨을 생성한다
	LPD3DXMESH createBoundingBox(const STBoundingBox &a_rstBoundingBox);

	//! 경계 구를 생성한다
	LPD3DXMESH createBoundingSphere(const STBoundingSphere &a_rstBoundingSphere);

public:
	void	setbDraw(BOOL a_bDraw) { m_bIsDraw = a_bDraw; }


private:			// private 변수

	STBoundingBox m_stBoundingBox;
	STBoundingSphere m_stBoundingSphere;

	LPD3DXMESH m_pBoundingBox = nullptr;
	LPD3DXMESH m_pBoundingSphere = nullptr;

	CRenderObject *m_pRenderObject = nullptr;
	EDebugDrawType m_eDebugDrawType = EDebugDrawType::NONE;

	BOOL	m_bIsDraw = true;
};
