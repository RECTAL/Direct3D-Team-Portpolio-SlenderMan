#include "CSpriteObject_Default.h"

CSpriteObject_Default::CSpriteObject_Default(std::string a_stSpriteTexturePath, std::string a_stExtantion, const float width, const float height, int a_nAnimationFrame)
	:CSpriteObject(a_stSpriteTexturePath, a_stExtantion,width,height, a_nAnimationFrame)
{
	m_ECWindowType = CWindowType::NONE;
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
