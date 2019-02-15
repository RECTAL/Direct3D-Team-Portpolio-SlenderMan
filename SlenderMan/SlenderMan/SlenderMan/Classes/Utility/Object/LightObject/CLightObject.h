#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CObject.h"

//! ����
class CLightObject : public CObject
{
public:			// constructor , destructor
	CLightObject(int a_nIndex);
	virtual ~CLightObject();
public:			// interface 
	virtual void update(void) override;						//! ���¸� �����Ѵ�

public:			// getter, setter
	/************************************************/
	//getter
	/************************************************/
	bool getLightEnable(void);								//! Ȱ�� ���θ� ��ȯ�Ѵ�
	D3DXMATRIXA16 getViewMatrix(void);						//! �� ����� ��ȯ�Ѵ�


	/************************************************/
	//setter
	/************************************************/
	void setLightEnable(bool a_bIsEnable);					//! Ȱ�� ���θ� �����Ѵ�
	void setDiffuseColor(const D3DXCOLOR &a_rstColor);		//! ���ݻ� ������ �����Ѵ�


protected:			

	int m_nIndex = 0;
	bool m_bIsEnable = false;

	D3DLIGHT9 m_stLight;
	D3DXMATRIXA16 m_stViewMatrix;
};
