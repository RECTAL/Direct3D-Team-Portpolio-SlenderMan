#pragma once
#include "../../../Define/KGlobalDefine.h"
#include "../../../Utility/Base/CScene.h"

class CSpriteObject_Default;

class CVictoryScene :public CScene
{
public:		//constructor , destructor
	CVictoryScene(std::string a_stSceneName);
	virtual ~CVictoryScene();

public:		//interface
	virtual void update(void)override;				//interface : IUpdateable
	virtual void draw(void)override;				//interface : IRenderable
	virtual void drawUI(void)override;				//interface : IRenderable

public:		//public override function
	virtual void init()override;					//override	: CScene


public:
	void createSound(void);

private:
	CSpriteObject_Default* background = nullptr;

	bool isFirst = true;
	bool m_bIsStartSound = true;;
};