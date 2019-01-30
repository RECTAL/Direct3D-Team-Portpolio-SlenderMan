#pragma once

#include "ILoader.h"

class GameOverSceneLoader :public ILoader
{
public:
	virtual void loadResources_Init();					//초기화 다운
	virtual void loadResources_Effects();				//이펙트 리소스 다운
	virtual void loadResources_Sounds();				//사운드 리소스 다운
	virtual void loadResources_Meshes();				//정적메시,스킨드 메시 다운
};