#pragma once
#include "ILoader.h"

class MainPlaySceneLoader :public ILoader
{
public:
	virtual void loadResources_Texture();				//�ؽ���	���ҽ� �ٿ�
	virtual void loadResources_Effects();				//����Ʈ ���ҽ� �ٿ�
	virtual void loadResources_Sounds();				//���� ���ҽ� �ٿ�
	virtual void loadResources_Meshes();				//�����޽�,��Ų�� �޽� �ٿ�
};