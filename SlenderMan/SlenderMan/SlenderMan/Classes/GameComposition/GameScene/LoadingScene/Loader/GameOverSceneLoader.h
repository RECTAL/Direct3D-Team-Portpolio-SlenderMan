#pragma once

#include "ILoader.h"

class GameOverSceneLoader :public ILoader
{
public:
	virtual void loadResources_Init();					//�ʱ�ȭ �ٿ�
	virtual void loadResources_Effects();				//����Ʈ ���ҽ� �ٿ�
	virtual void loadResources_Sounds();				//���� ���ҽ� �ٿ�
	virtual void loadResources_Meshes();				//�����޽�,��Ų�� �޽� �ٿ�
};