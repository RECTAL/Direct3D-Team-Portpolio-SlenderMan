#pragma once
#include "../../../../Define/KGlobalDefine.h"

class ILoader
{
public:
	virtual void loadResources_Init()=0;				//�ʱ�ȭ �ٿ�
	virtual void loadResources_Effects()=0;				//����Ʈ ���ҽ� �ٿ�
	virtual void loadResources_Sounds()=0;				//���� ���ҽ� �ٿ�
	virtual void loadResources_Meshes()=0;				//�����޽�,��Ų�� �޽� �ٿ�

public:
	void CheckAllDownLoad()
	{
		IsAllDownLoad(
			NULL,
			m_bIsInitDownLoad&&m_bIsEffectsDownLoad&&m_bIsSoundsDownLoad&&m_bIsMeshesDownLoad,
			false
		);
	};

public:
	void IsAllDownLoad(bool* isOut,bool isIn,bool isGet)
	{
		m_stMutex.lock();
		if (isGet)
		{
			*isOut = m_bIsAllDownLoad;
		}
		else
		{
			m_bIsAllDownLoad = isIn;
		}
		m_stMutex.unlock();
	}

protected:
	bool	m_bIsInitDownLoad		= false;
	bool	m_bIsEffectsDownLoad	= false;
	bool	m_bIsSoundsDownLoad		= false;
	bool	m_bIsMeshesDownLoad		= false;

	bool	m_bIsAllDownLoad = false;
	std::string m_stLoadResourcesName;
	std::mutex m_stMutex;
};