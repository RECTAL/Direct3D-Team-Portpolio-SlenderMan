#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CObject.h"

//! ī�޶�
class CCameraObject : public CObject
{
public:			// constructor, destructor

	CCameraObject(float a_fAspect);


public:			// getter, setter
	/*********************************************/
	//getter
	/*********************************************/
	D3DXMATRIXA16 getViewMatrix(void);					//! �� ����� ��ȯ�Ѵ�
	D3DXMATRIXA16 getProjectionMatrix(void);			//! ���� ����� ��ȯ�Ѵ�


	/*********************************************/
	//setter
	/*********************************************/
	void setAspect(float a_fAspect);					//! ��Ⱦ�� �����Ѵ�

public:			// interface 
	virtual void update(void) override;					//! ���¸� �����Ѵ�

protected:		

	D3DXMATRIXA16 m_stViewMatrix;
	D3DXMATRIXA16 m_stProjectionMatrix;
};
