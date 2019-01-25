#pragma once
#include "../CSpriteObject.h"
#include "../../../Base/CWindow.h"

class CWindow;
class CSpriteObject_Button :public CSpriteObject
{
public:
	CSpriteObject_Button(std::string a_stSpriteTexturePath, std::string a_stExtantion, int a_nAnimationFrame);
	virtual ~CSpriteObject_Button();
public:			//interface
	virtual void update()override;
	virtual void doDrawUI()override;

public:			//getter,setter
	/******************************************************/
	//getter
	/******************************************************/
	CWindow*		getWindow() { return m_pWindow; }

public:
	virtual void init(std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc);
	virtual void release();
private:
	CWindow*		m_pWindow;
};