#pragma once

#include "../../Define/KGlobalDefine.h"
#include "CObject.h"
#include "../Interface/IRenderable.h"

class CDebugDraw;

//! 기본 렌더링 객체
class CRenderObject : public CObject,
	public IRenderable
{
public:			// 인터페이스 구현

	//! 그린다
	virtual void draw(void) final override;

public:			// getter, setter

	//! 그리기 여부를 반환한다
	bool getVisible(void);

	//! 경계 볼륨을 반환한다
	STBoundingBox getBoundingBox(void);

	//! 객체 볼륨을 반환한다
	STObjectBox getObjectBox(void);

	//! 최종 경계 볼륨을 반환한다
	STBoundingBox getFinalBoundingBox(void);

	//! 경계 구를 반환한다
	STBoundingSphere getBoundingSphere(void);

	//! 최종 경계 구를 반환한다
	STBoundingSphere getFinalBoundingSphere(void);

	bool&			getbIsCollision() { return m_bIsCollision; }

	bool&			getbOutLineDraw() { return m_bIsOutlineDraw; }

	bool&			getbIsDraw() { return m_bIsDraw; }

	EObjType&		getObjType() { return m_eObjType; }
	EObjClasses&	getObjClasses() { return m_eClasses; }
	//! 그리기 여부를 변경한다
	void setVisible(bool a_bIsVisible);

	//! 디버그 여부를 변경한다
	void setDebugEnable(bool a_bIsDebugEnable,
		EDebugDrawType a_eDebugDrawType = EDebugDrawType::BOX);

	//! 경계 볼륨을 변경한다
	void setBoundingBox(const STBoundingBox &a_rstBoundingBox);

	//! 경계 구를 변경한다
	void setBoundingSphere(const STBoundingSphere &a_rstBoundingSphere);

protected:			// protected 함수

	//! 물체를 그리기 전
	virtual void preDraw(void);

	//! 물체를 그린다
	virtual void doDraw(void);

	//! 물체를 그린 후
	virtual void postDraw(void);

protected:			// 생성자

	//! 생성자
	CRenderObject(void);

protected:			// protected 변수

	bool m_bIsVisible = false;
	bool m_bIsDebugEnable = false;
	bool m_bIsCollision = false;
	bool m_bIsOutlineDraw = false;
	bool m_bIsDraw = false;

	CDebugDraw *m_pDebugDraw = nullptr;
	EObjType	m_eObjType;
	EObjClasses m_eClasses;

	STBoundingBox m_stBoundingBox;
	STBoundingSphere m_stBoundingSphere;
};
