#pragma once
#include "../../Define/KGlobalDefine.h"
#include "../../Utility/Interface/IUpdateable.h"
#include "../../Utility/Interface/IRenderable.h"

class CSpriteObject;
class CWindow:public IUpdateable,IRenderable
{
public:
	CWindow(std::string a_stWindowName, CWindowType a_ECWindowType);
	virtual ~CWindow();

public:				//interface
	virtual void update(void)override;			//interface : IUpdateable
	virtual void draw(void)override;			//interface : IRenderable
public:				
	virtual void	init(std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, CSpriteObject* a_pSpriteObject);
	virtual void	release();
	virtual void	createBeginCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr) = 0;
	virtual void	createCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr) = 0;
	virtual void	createEndCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr) = 0;
	virtual void	setIsActive(bool isActive, POINT a_stMousePosition) = 0;
public:				//getter,setter
	/******************************************************/
	//getter
	/******************************************************/
	CWindowType		getWindowType() { return m_eCWindowType; }
	D3DXVECTOR3		getAbsolutePosition() { return m_stAbsolutePosition; }
	D3DXVECTOR3		getRelativePosition() { return m_stRelativePosition; }
	SIZE			getActiveSize() { return m_stActiveSize; }

	bool			getIsActive() { return m_bIsActive; }

	std::function<void(void)>&	getBeginCallBackFunc() { return m_stBeginCallBackFunc; }
	std::function<void(void)>&	getCallBackFunc() { return m_stCallBackFunc; }
	std::function<void(void)>&	getEndCallBackFunc() { return m_stEndCallBackFunc; }

	/******************************************************/
	//setter
	/******************************************************/
	void			setAbsolutePosition(D3DXVECTOR3 a_stPosition) { m_stAbsolutePosition = a_stPosition; }
	void			setRelativePosition(D3DXVECTOR3 a_stPosition,SIZE a_stParentArea)
						{
							m_stRelativePosition = a_stPosition; 
							m_stRelativePosition.x = min(a_stParentArea.cx/2, m_stRelativePosition.x);
							m_stRelativePosition.x = max(-a_stParentArea.cx / 2, m_stRelativePosition.x);
	
							m_stRelativePosition.y = min(a_stParentArea.cy/2, m_stRelativePosition.y);
							m_stRelativePosition.y = max(-a_stParentArea.cy / 2, m_stRelativePosition.y);
						}

public:
	void			addParentWindow(CWindow* a_pParentWindow) { m_pParentWindow = a_pParentWindow; }
	void			addChildWindow(CWindow*	a_pChildWindow)
					{ 
						a_pChildWindow->addParentWindow(this);
						m_oChildWindow.push_back(a_pChildWindow); 
					}


protected:
	std::string				m_stWindowName;
	CWindowType				m_eCWindowType;

	D3DXVECTOR3				m_stAbsolutePosition;
	D3DXVECTOR3				m_stRelativePosition;
	SIZE					m_stActiveSize;



	CWindow*				m_pParentWindow = nullptr;
	std::vector<CWindow*>	m_oChildWindow;

	std::function<void(void)> m_stBeginCallBackFunc = nullptr;
	std::function<void(void)> m_stCallBackFunc = nullptr;
	std::function<void(void)> m_stEndCallBackFunc = nullptr;

	CSpriteObject*			m_pSpriteObject;

	bool					m_bPreActive = false;
	bool					m_bIsActive = false;
};