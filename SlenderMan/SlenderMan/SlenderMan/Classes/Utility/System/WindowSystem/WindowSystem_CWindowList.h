#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CWindow.h"


class CWindowList :public CWindow
{
public:
	CWindowList(std::string a_stWindowName, CWindowType a_ECWindowType, SIZE a_stActiveSize, D3DXVECTOR3 a_stAbsolutePos);
	virtual ~CWindowList();

public:			//interface
	virtual void update(void)override;			//interface : IUpdateable

public:			//override
	virtual void init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc,
		std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc)final override;
	virtual void release()final override;
	virtual void createCrashCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr)override;
	virtual void createBeginCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr)override;
	virtual void createCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr) override;
	virtual void createEndCallBackFunc(std::function<void(void)>* a_pCallBackFunc = nullptr) override;
	virtual void createActiveRect()override;
private:
	
};