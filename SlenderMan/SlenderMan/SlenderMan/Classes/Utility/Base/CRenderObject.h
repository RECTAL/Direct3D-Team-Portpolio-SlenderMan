#pragma once

#include "../../Define/KGlobalDefine.h"
#include "CObject.h"
#include "../Interface/IRenderable.h"

class CDebugDraw;

//! �⺻ ������ ��ü
class CRenderObject : public CObject,
	public IRenderable
{
public:			// �������̽� ����

	//! �׸���
	virtual void draw(void) final override;

public:			// getter, setter

	//! �׸��� ���θ� ��ȯ�Ѵ�
	bool getVisible(void);

	//! ��� ������ ��ȯ�Ѵ�
	STBoundingBox getBoundingBox(void);

	//! ��ü ������ ��ȯ�Ѵ�
	STObjectBox getObjectBox(void);

	//! ���� ��� ������ ��ȯ�Ѵ�
	STBoundingBox getFinalBoundingBox(void);

	//! ��� ���� ��ȯ�Ѵ�
	STBoundingSphere getBoundingSphere(void);

	//! ���� ��� ���� ��ȯ�Ѵ�
	STBoundingSphere getFinalBoundingSphere(void);

	bool&			getbIsCollision() { return m_bIsCollision; }

	bool&			getbOutLineDraw() { return m_bIsOutlineDraw; }

	bool&			getbIsDraw() { return m_bIsDraw; }

	EObjType&		getObjType() { return m_eObjType; }
	EObjClasses&	getObjClasses() { return m_eClasses; }
	//! �׸��� ���θ� �����Ѵ�
	void setVisible(bool a_bIsVisible);

	//! ����� ���θ� �����Ѵ�
	void setDebugEnable(bool a_bIsDebugEnable,
		EDebugDrawType a_eDebugDrawType = EDebugDrawType::BOX);

	//! ��� ������ �����Ѵ�
	void setBoundingBox(const STBoundingBox &a_rstBoundingBox);

	//! ��� ���� �����Ѵ�
	void setBoundingSphere(const STBoundingSphere &a_rstBoundingSphere);

protected:			// protected �Լ�

	//! ��ü�� �׸��� ��
	virtual void preDraw(void);

	//! ��ü�� �׸���
	virtual void doDraw(void);

	//! ��ü�� �׸� ��
	virtual void postDraw(void);

protected:			// ������

	//! ������
	CRenderObject(void);

protected:			// protected ����

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
