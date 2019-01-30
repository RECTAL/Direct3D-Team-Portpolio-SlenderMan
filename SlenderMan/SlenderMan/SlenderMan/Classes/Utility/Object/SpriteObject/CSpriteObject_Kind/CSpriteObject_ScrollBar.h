#pragma once
#include "../CSpriteObject.h"
#include "../../../Base/CWindow.h"

class CWindow;
class CSpriteObject_Button;
class CSpriteObject_ScrollBar :public CSpriteObject
{
public:
	CSpriteObject_ScrollBar(std::string a_stSpriteTexturePath, std::string a_stExtantion, const float width, const float height, int a_nAnimationFrame);
	virtual ~CSpriteObject_ScrollBar();
public:			//interface
	virtual void update()override;
	virtual void doDrawUI()override;

public:			//getter,setter
	/******************************************************/
	//getter
	/******************************************************/
	CWindow*		getWindow() { return m_pWindow; }
	D3DXVECTOR3&	getRelativePos() { return m_stRelativePos; }
	float			getSetValue() { return m_fSetVal; }

public:
	virtual void init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc,float minVal, float maxVal, CSpriteObject_Button* a_pSpriteButton, bool a_bIsChild = false, D3DXVECTOR3 a_rstRelativePos = D3DXVECTOR3(0, 0, 0));
	virtual void release();

private:
	void calSetValue();

private:
	CWindow*		m_pWindow;
	bool			m_bIsChild;
	D3DXVECTOR3		m_stRelativePos;


	float			m_fMinVal;
	float			m_fMaxVal;
	float			m_fSetVal;

	CSpriteObject_Button*	m_pSpriteButton = nullptr;
	D3DXVECTOR3		m_stMoveOffset;
};