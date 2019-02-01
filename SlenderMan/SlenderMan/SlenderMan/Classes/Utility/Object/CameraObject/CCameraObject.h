#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CObject.h"

//! 카메라
class CFrustum;
class CCameraObject : public CObject
{
public:			// constructor, destructor

	CCameraObject(float a_fAspect);


public:			// getter, setter
	/*********************************************/
	//getter
	/*********************************************/
	D3DXMATRIXA16 getViewMatrix(void);					//! 뷰 행렬을 반환한다
	D3DXMATRIXA16 getProjectionMatrix(void);			//! 투영 행렬을 반환한다
	CFrustum*	  getCameraFrustum() { return m_pFrustum; }

	/*********************************************/
	//setter
	/*********************************************/
	void setAspect(float a_fAspect);					//! 종횡비를 변경한다

public:			// interface 
	virtual void update(void) override;					//! 상태를 갱신한다

protected:		

	D3DXMATRIXA16 m_stViewMatrix;
	D3DXMATRIXA16 m_stProjectionMatrix;
	CFrustum*	  m_pFrustum;
};
