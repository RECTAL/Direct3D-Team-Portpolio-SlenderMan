#pragma once
#include "../CSpriteObject.h"
#include "../../../Base/CWindow.h"

class CWindow;
class CSpriteObject_Button :public CSpriteObject
{
public:
	CSpriteObject_Button(std::string a_stSpriteTexturePath, std::string a_stExtantion, const float width, const float height, int a_nAnimationFrame, bool a_bIsCheckBox = false);
	virtual ~CSpriteObject_Button();
public:			//interface
	virtual void update()override;
	virtual void doDrawUI()override;

public:			//getter,setter
	/******************************************************/
	//getter
	/******************************************************/
	CWindow*		getWindow() { return m_pWindow; }
	D3DXVECTOR3&	getRelativePos() { return m_stRelativePos; }

public:
	virtual void init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, bool a_bIsChild = false, D3DXVECTOR3 a_rstRelativePos = D3DXVECTOR3(0,0,0));
	virtual void release();
private:
	CWindow*		m_pWindow;
	bool			m_bIsChild;
	bool			m_bIsCheckBox = false;
	D3DXVECTOR3		m_stRelativePos;
};