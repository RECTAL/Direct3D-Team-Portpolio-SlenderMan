#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CObject.h"

//! 광원
class CLightObject : public CObject
{
public:			// constructor , destructor
	CLightObject(int a_nIndex);
	virtual ~CLightObject();
public:			// interface 
	virtual void update(void) override;						//! 상태를 갱신한다

public:			// getter, setter
	/************************************************/
	//getter
	/************************************************/
	bool getLightEnable(void);								//! 활성 여부를 반환한다
	D3DXMATRIXA16 getViewMatrix(void);						//! 뷰 행렬을 반환한다


	/************************************************/
	//setter
	/************************************************/
	void setLightEnable(bool a_bIsEnable);					//! 활성 여부를 변경한다
	void setDiffuseColor(const D3DXCOLOR &a_rstColor);		//! 난반사 색상을 변경한다


protected:			

	int m_nIndex = 0;
	bool m_bIsEnable = false;

	D3DLIGHT9 m_stLight;
	D3DXMATRIXA16 m_stViewMatrix;
};
