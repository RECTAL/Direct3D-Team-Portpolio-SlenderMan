#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CWindow.h"


class CWindowContainer :public CWindow
{
public:
	CWindowContainer(std::string a_stWindowName, CWindowType a_ECWindowType,SIZE a_stActiveSize);
	virtual ~CWindowContainer();

public:			//interface
	virtual void update(void)override;			//interface : IUpdateable
	virtual void draw(void)override;			//interface	: IRenderable

public:			//override
	virtual void init(
		std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, CSpriteObject* a_pSpriteObject)final override;
	virtual void release()final override;
	virtual void createBeginCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr)override;
	virtual void createCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr) override;
	virtual void createEndCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr) override;
	virtual void setIsActive(bool isActive, POINT a_stMousePosition)override;

private:
	POINT	m_stPreOffset;
	POINT	m_stOffset;
};