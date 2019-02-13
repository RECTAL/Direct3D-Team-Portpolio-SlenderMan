#pragma once

#include "../../Define/KGlobalDefine.h"
#include "../Interface/IUpdateable.h"

//! CObject
class CObject : public IUpdateable
{
public:			// constructor, destructor

	CObject(void);								//! constructor
	virtual ~CObject(void);						//! destructor


public:			// interface

	virtual void update(void) override;			//! interface :IUpdatealbe

public:			// getter, setter

	/***************************************************/
	//getter
	/***************************************************/
	D3DXVECTOR3 getPosition(void);				//! get Position
	D3DXVECTOR3 getScale(void);					//! get Scale
	D3DXVECTOR3 getRightDirection(void);		//! get X axis
	D3DXVECTOR3 getUpDirection(void);			//! get Y axis
	D3DXVECTOR3 getForwardDirection(void);		//! get Z axis
	D3DXMATRIXA16 getWorldMatrix(void);			//! get World Matrix
	D3DXMATRIXA16 getFinalWorldMatrix(void);	//! get Final Matrix


	/***************************************************/
	//setter
	/***************************************************/
	void setParentObject(CObject *a_pParentObject);					//! set Parent
	void setPosition(const D3DXVECTOR3 &a_rstPosition);				//! set Position
	void setScale(const D3DXVECTOR3 &a_rstScale);					//! set Scale
	void setRotation(const D3DXVECTOR3 &a_rstRotation);				//! set Rotation
	void setOffsetMatrix(const D3DXMATRIXA16 &a_rstOffsetMatrix);	//! set OffsetMatrix
	void setForwardDirection(D3DXVECTOR3& a_stDirection);
	void setUpDirection(D3DXVECTOR3& a_stDirection);
	void setRightDirection(D3DXVECTOR3& a_stDirection);


public:			//public function : utility	
	/***************************************************/
	//public function : about move
	/***************************************************/
	void moveByXAxis(float a_fDistance, bool a_bIsLocal = true);	
	void moveByYAxis(float a_fDistance, bool a_bIsLocal = true);
	void moveByZAxis(float a_fDistance, bool a_bIsLocal = true);


	/***************************************************/
	//public function : about rotate
	/***************************************************/
	void rotateByXAxis(float a_fAngle, bool a_bIsLocal = true);
	void rotateByYAxis(float a_fAngle, bool a_bIsLocal = true);
	void rotateByZAxis(float a_fAngle, bool a_bIsLocal = true);
	void rotateByPosition(const D3DXVECTOR3 &a_rstPosition, bool a_bIsSphere = true);
	void rotateByDirection(const D3DXVECTOR3 &a_rstDirection, bool a_bIsSphere = true);
	void rotateByMatrix(const D3DXMATRIXA16 &a_rstMatrix);


	/***************************************************/
	//public function : about parent<->child
	/***************************************************/
	void addChildObject(CObject *a_pChildObject);
	void removeChildObject(CObject *a_pChildObject);

private:
	void resetAxis(void);					


protected:	

	D3DXVECTOR3 m_stPosition;
	D3DXVECTOR3 m_stScale;

	D3DXVECTOR3 m_stRightDirection;
	D3DXVECTOR3 m_stUpDirection;
	D3DXVECTOR3 m_stForwardDirection;

	D3DXMATRIXA16 m_stWorldMatrix;
	D3DXMATRIXA16 m_stOffsetMatrix;

	CObject *m_pParentObject = nullptr;
	std::vector<CObject *> m_oChildObjectList;
};
