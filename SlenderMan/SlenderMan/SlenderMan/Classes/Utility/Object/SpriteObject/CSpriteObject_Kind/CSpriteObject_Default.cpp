#include "CSpriteObject_Default.h"

CSpriteObject_Default::CSpriteObject_Default(std::string a_stSpriteTexturePath, std::string a_stExtantion, const float width, const float height, int a_nAnimationFrame, bool a_bIsLoop)
	:CSpriteObject(a_stSpriteTexturePath, a_stExtantion, width, height, a_nAnimationFrame)
{
	m_ECWindowType = CWindowType::NONE;
	m_bIsLoop = a_bIsLoop;
	m_bIsLast = false;
}

CSpriteObject_Default::~CSpriteObject_Default()
{
	this->release();
}

void CSpriteObject_Default::update()
{
	////////////////////////////////////////////
	// �����Ͻ�
	////////////////////////////////////////////
	if (m_bIsLoop) {
		CUIObject::update();

		if (m_nTextureOffset >= m_oSpriteTexture.size()) {
			m_bIsLast = true;
		}
		if (m_bIsLast && m_nTextureOffset >= 0) {
			m_nTextureOffset--;
		}
		if (m_nTextureOffset <= m_oSpriteTexture.size()){
			m_bIsLast = false;
			m_nTextureOffset++;
		}
		
	}
	////////////////////////////////////////////
	// �⺻ ��������Ʈ
	////////////////////////////////////////////
	else {
		CSpriteObject::update();
	}
}

void CSpriteObject_Default::doDrawUI()
{
	CSpriteObject::doDrawUI();
}
