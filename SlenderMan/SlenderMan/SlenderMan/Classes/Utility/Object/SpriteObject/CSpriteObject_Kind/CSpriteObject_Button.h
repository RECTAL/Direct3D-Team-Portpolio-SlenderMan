#pragma once
#include "../CSpriteObject.h"

class CSpriteObject_Button :public CSpriteObject
{
public:
	CSpriteObject_Button(std::string a_stSpriteTexturePath, int a_nAnimationFrame);
	virtual ~CSpriteObject_Button();
public:			//interface

	virtual void update()override;
	virtual void doDrawUI()override;




};