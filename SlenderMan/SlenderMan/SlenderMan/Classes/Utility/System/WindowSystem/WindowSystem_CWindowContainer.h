#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CWindow.h"


class CWindowContainer :public CWindow
{
public:
	CWindowContainer(std::string a_stWindowName, CWindowType a_ECWindowType,SIZE a_stActiveSize, D3DXVECTOR3 a_stAbsolutePos);
	virtual ~CWindowContainer();

public:			//interface
	virtual void update(void)override;			//interface : IUpdateable

public:			//override
	virtual void init(
		std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc)final override;
	virtual void release()final override;
	virtual void createBeginCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr)override;
	virtual void createCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr) override;
	virtual void createEndCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr) override;

private:
	POINT	m_stPreOffset;
	POINT	m_stOffset;
	LONG	m_stDeltaX =0;
	LONG	m_stDeltaY =0;
};