#include "CSpriteObject_Button.h"
#include "../../../Manager/CResourceManager.h"
#include "../../../Manager/CInputManager.h"
#include "../../../System/WindowSystem/WindowSystem_CWindowButton.h"
CSpriteObject_Button::CSpriteObject_Button(std::string a_stSpriteTexturePath, std::string a_stExtantion, int a_nAnimationFrame)
	:CSpriteObject(a_stSpriteTexturePath, a_stExtantion,a_nAnimationFrame)
{
	m_ECWindowType = CWindowType::BUTTON;
}

CSpriteObject_Button::~CSpriteObject_Button()
{
	this->release();
}

void CSpriteObject_Button::update()
{
	CSpriteObject::update();
	D3DSURFACE_DESC pSurf;
	LPDIRECT3DTEXTURE9 pTex = m_oSpriteTexture[m_nTextureOffset];
	pTex->GetLevelDesc(0, &pSurf);


	m_pWindow->getActiveSize() = SIZE{ (LONG)pSurf.Width,(LONG)pSurf.Height};
	m_pWindow->update();
	setPosition(m_pWindow->getAbsolutePosition());

	if (PtInRect(&m_pWindow->getActiveRect(), GET_MOUSE_POSITION()))
	{

		if (IS_MOUSE_BUTTON_PRESSED(EMouseInput::LEFT))
		{
			m_pWindow->getBeginCallBackFunc()();
		}
		else if (IS_MOUSE_BUTTON_DOWN(EMouseInput::LEFT))
		{
			m_pWindow->getCallBackFunc()();
		}
		if (IS_MOUSE_BUTTON_RELEASED(EMouseInput::LEFT))
		{
			m_pWindow->getEndCallBackFunc()();
		}
	}
}

void CSpriteObject_Button::doDrawUI()
{
	CSpriteObject::doDrawUI();
}

void CSpriteObject_Button::init(std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, bool a_bIsChild, D3DXVECTOR3 a_rstRelativePos)
{
	D3DSURFACE_DESC pSurf;
	LPDIRECT3DTEXTURE9 pTex = m_oSpriteTexture[m_nTextureOffset];
	pTex->GetLevelDesc(0, &pSurf);
	m_pWindow = new CWindowButton("WindowButton", CWindowType::BUTTON, SIZE{ (LONG)pSurf.Width,(LONG)pSurf.Height }, getPosition());
	m_pWindow->init(a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc);
	m_bIsChild = a_bIsChild;
	m_stRelativePos = a_rstRelativePos;
}

void CSpriteObject_Button::release()
{
	SAFE_DELETE(m_pWindow);
}
