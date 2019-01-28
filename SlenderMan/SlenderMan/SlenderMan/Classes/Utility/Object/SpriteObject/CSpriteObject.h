#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CUIObject.h"


class CSpriteObject :public CUIObject
{
public:
	typedef std::vector<LPDIRECT3DTEXTURE9> oSpriteTexture;

public:
	CSpriteObject(std::string a_stSpriteTexturePath,std::string a_stExtantion, int a_nAnimationFrame);
	virtual ~CSpriteObject();
public:			//interface

	virtual void update()override;
	virtual void doDrawUI()override;

public:
	CWindowType	  getWindowType() { return m_ECWindowType; }

private:
	LPD3DXSPRITE   createSprite();


protected:
	void	release();

public:
	oSpriteTexture m_oSpriteTexture;
	LPD3DXSPRITE   m_pSprite = nullptr;
	CWindowType    m_ECWindowType;

	int m_nTextureOffset=0;
	int m_nAnimationCount = 0;
	std::string m_stSpriteTexturePath;
};