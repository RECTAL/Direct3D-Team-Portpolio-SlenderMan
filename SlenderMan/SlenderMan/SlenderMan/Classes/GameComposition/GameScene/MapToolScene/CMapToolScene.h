#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"


class CMapToolScene :public CScene
{
public:		//constructor , destructor
	CMapToolScene(std::string a_stSceneName);
	virtual ~CMapToolScene();

public:		//interface
	virtual void update(void)override;				//interface : IUpdateable
	virtual void draw(void)override;				//interface : IRenderable
	virtual void drawUI(void)override;				//interface : IRenderable

public:		//public override function
	virtual void init()override;					//override	: CScene




};