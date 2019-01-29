#pragma once
#include "../CSpriteObject.h"

class CSpriteObject_Default :public CSpriteObject
{
public:
	CSpriteObject_Default(std::string a_stSpriteTexturePath,std::string a_stExtantion, const float width, const float height, int a_nAnimationFrame);
	virtual ~CSpriteObject_Default();
public:			//interface
	virtual void update()override;
	virtual void doDrawUI()override;

};