#include "CSpriteObject.h"
#include "../../Base/CDirect3DApplication.h"
#include "../../Manager/CDeviceManager.h"
#include "../../Manager/CResourceManager.h"

CSpriteObject::CSpriteObject(std::string a_stSpriteTexturePath, int a_nAnimationFrame)
	:m_stSpriteTexturePath(a_stSpriteTexturePath),m_nAnimationCount(a_nAnimationFrame)
{
	m_pSprite = this->createSprite();
	for (int i = 0; i < m_nAnimationCount; i++)
	{
		char path[MAX_PATH];
		sprintf(path, "%s_%d", a_stSpriteTexturePath.c_str(), i);
		m_oSpriteTexture.push_back(GET_SPRITE_TEXTURE(path));
	}
}

CSpriteObject::~CSpriteObject()
{
	this->release();
}

void CSpriteObject::release()
{
	SAFE_RELEASE(m_pSprite);
}

void CSpriteObject::update()
{
	CUIObject::update();
	m_nTextureOffset++;
	if (m_nTextureOffset >= m_oSpriteTexture.size())m_nTextureOffset = 0;
}

void CSpriteObject::doDrawUI()
{

	D3DXMATRIXA16 stWorldMatrix;
	D3DXMatrixScaling(&stWorldMatrix, this->getScale().x, this->getScale().y, 1.0f);

	D3DSURFACE_DESC stSurfaceDesc;
	m_oSpriteTexture[m_nTextureOffset]->GetLevelDesc(0, &stSurfaceDesc);

	RECT stTextureRect = {
		0, 0, stSurfaceDesc.Width, stSurfaceDesc.Height
	};

	D3DXVECTOR3 stPosition = {
		this->getPosition().x * (1.0f / m_stScale.x),
		this->getPosition().y * (1.0f / m_stScale.y),
		this->getPosition().z
	};

	m_pSprite->Draw(m_oSpriteTexture[m_nTextureOffset],
		&stTextureRect,
		&D3DXVECTOR3(stTextureRect.right / 2.0f, stTextureRect.bottom / 2.0f, 0.0f),
		&stPosition,
		m_nDiffuseColor);
}

LPD3DXSPRITE CSpriteObject::createSprite()
{
	LPD3DXSPRITE pSprite = nullptr;
	D3DXCreateSprite(
		GET_DEVICE(),
		&pSprite
	);

	return pSprite;
}
