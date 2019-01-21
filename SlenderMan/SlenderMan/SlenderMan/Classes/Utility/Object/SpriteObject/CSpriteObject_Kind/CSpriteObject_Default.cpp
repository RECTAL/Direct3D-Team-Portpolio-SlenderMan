#include "CSpriteObject_Default.h"

CSpriteObject_Default::CSpriteObject_Default(std::string a_stSpriteTexturePath, std::string a_stExtantion, int a_nAnimationFrame)
	:CSpriteObject(a_stSpriteTexturePath, a_stExtantion,a_nAnimationFrame)
{
}

CSpriteObject_Default::~CSpriteObject_Default()
{
	this->release();
}


void CSpriteObject_Default::update()
{
	CSpriteObject::update();
}

void CSpriteObject_Default::doDrawUI()
{
	CSpriteObject::doDrawUI();
}
