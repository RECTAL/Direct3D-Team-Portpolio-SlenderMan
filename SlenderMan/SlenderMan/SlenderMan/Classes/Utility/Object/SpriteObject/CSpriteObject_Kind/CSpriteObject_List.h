#pragma once
#include "../CSpriteObject.h"
#include "../../../Base/CWindow.h"

class CWindow;
class CSpriteObject_List :public CSpriteObject
{
public:
	CSpriteObject_List(std::string a_stSpriteTexturePath, std::string a_stExtantion,const float width, const float height, int a_nAnimationFrame );
	virtual ~CSpriteObject_List();
public:			//interface
	virtual void update()override;
	virtual void doDrawUI()override;

public:			//getter,setter
	/******************************************************/
	//getter
	/******************************************************/
	CWindow*		getWindow() { return m_pWindow; }
	bool			getIsChild() { return m_bIsChild; }
	D3DXVECTOR3&	getRelativePos() { return m_stRelativePos; }
	D3DXVECTOR3&	getMoveOffset() { return m_stMoveOffset; }

	std::vector<std::pair<std::string, CSpriteObject*>>&	getChildSpriteObjectList() { return m_oChildSpriteObjectList; }

	/******************************************************/
	//setter
	/******************************************************/
	virtual void setVisible(bool a_bIsVisible) override;
public:
	virtual void init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc,
		std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, bool a_bIsChild = false, D3DXVECTOR3 a_rstRelativePos = D3DXVECTOR3(0, 0, 0));
	virtual void release();

	void		addChildSpriteObject(std::string a_stSpriteName, CWindowType a_ECWindowType, CSpriteObject* a_pSpriteObject);
	void		deleteChildSpriteObject(std::string a_stSpriteName);
private:
	CWindow*		m_pWindow;
	D3DXVECTOR3		m_stRelativePos;

	D3DXVECTOR3		m_stListOffset;
	D3DXVECTOR3		m_stMoveOffset;
	bool			m_bIsChild;
	std::vector<std::pair<std::string, CSpriteObject*>>	m_oChildSpriteObjectList;
};