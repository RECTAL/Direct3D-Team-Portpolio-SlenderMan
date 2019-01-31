#include "CRendertargetManager.h"
#include "../Manager/CDeviceManager.h"
#include "../Manager/CWindowManager.h"
CRendertargetManager::~CRendertargetManager()
{
	for (auto oIter : m_oRenderTargetList)
	{
		SAFE_DELETE(oIter.second);
	}
	m_oRenderTargetList.clear();
}

void CRendertargetManager::init()
{
	GET_DEVICE()->GetRenderTarget(0, &m_stOriginRenderTarget.m_stRenderTarget.m_pTexSurf);
	GET_DEVICE()->GetDepthStencilSurface(&m_stOriginRenderTarget.m_stRenderTarget.m_pDepthStencil);
	GET_DEVICE()->GetViewport(&m_stOriginRenderTarget.m_stRenderTarget.m_stViewPort);

	m_stOriginRenderTarget.m_stRenderTarget.m_nWidth = GET_WINDOW_SIZE().cx;
	m_stOriginRenderTarget.m_stRenderTarget.m_nHeight = GET_WINDOW_SIZE().cy;
	m_stOriginRenderTarget.m_stRenderTarget.m_pTex = nullptr;
	
}

void CRendertargetManager::addRenderTarget(std::string a_stTargetName, CRenderTarget * m_pRenderTarget)
{
	if (m_pRenderTarget != nullptr)
	{
		auto iter = m_oRenderTargetList.find(a_stTargetName);
		if (iter == m_oRenderTargetList.end())
		{
			m_oRenderTargetList.insert(decltype(m_oRenderTargetList)::value_type(a_stTargetName, m_pRenderTarget));
		}
	}
}

void CRendertargetManager::delRenderTarget(std::string a_stTargetName)
{
	auto iter = m_oRenderTargetList.find(a_stTargetName);
	if (iter != m_oRenderTargetList.end())
	{
		SAFE_DELETE(iter->second);
		m_oRenderTargetList.erase(iter);
	}
}

CRenderTarget * CRendertargetManager::findRenderTarget(std::string a_stTargetName)
{
	auto iter = m_oRenderTargetList.find(a_stTargetName);
	if (iter != m_oRenderTargetList.end())
	{
		return iter->second;
	}
	return nullptr;
}


void CRendertargetManager::resetRenderTarget()
{
	GET_DEVICE()->SetRenderTarget(0, m_stOriginRenderTarget.m_stRenderTarget.m_pTexSurf);
	GET_DEVICE()->SetDepthStencilSurface(m_stOriginRenderTarget.m_stRenderTarget.m_pDepthStencil);
	GET_DEVICE()->SetViewport(&m_stOriginRenderTarget.m_stRenderTarget.m_stViewPort);
}