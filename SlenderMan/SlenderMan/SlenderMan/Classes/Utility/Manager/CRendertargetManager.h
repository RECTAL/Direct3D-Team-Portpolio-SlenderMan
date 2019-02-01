#pragma once
#include "../../Define/KGlobalDefine.h"
#include "../../Utility/System/RenderSystem/RenderSystem_CRenderTarget.h"


class CRendertargetManager
{
private:
	typedef std::unordered_map<std::string, CRenderTarget*> oRTMap;
public:
	DECLARE_SINGLETON(CRendertargetManager);

private:
	~CRendertargetManager();

public:
	CRenderTarget getOriginRenderTarget() { return m_stOriginRenderTarget; }

public:
	void init();
	

	void addRenderTarget(std::string a_stTargetName, CRenderTarget* m_pRenderTarget);
	void delRenderTarget(std::string a_stTargetName);
	
	void resetRenderTarget();
	
	CRenderTarget* findRenderTarget(std::string a_stTargetName);


private:
	oRTMap		   m_oRenderTargetList;
	CRenderTarget  m_stOriginRenderTarget;
};