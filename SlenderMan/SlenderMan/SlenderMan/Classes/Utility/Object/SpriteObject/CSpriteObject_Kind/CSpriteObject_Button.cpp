#include "CSpriteObject_Button.h"

CSpriteObject_Button::CSpriteObject_Button(std::string a_stSpriteTexturePath, std::string a_stExtantion, int a_nAnimationFrame)
	:CSpriteObject(a_stSpriteTexturePath, a_stExtantion,a_nAnimationFrame)
{
}

CSpriteObject_Button::~CSpriteObject_Button()
{
	this->release();
}

void CSpriteObject_Button::update()
{
	CSpriteObject::update();
}

void CSpriteObject_Button::doDrawUI()
{
	CSpriteObject::doDrawUI();
}
