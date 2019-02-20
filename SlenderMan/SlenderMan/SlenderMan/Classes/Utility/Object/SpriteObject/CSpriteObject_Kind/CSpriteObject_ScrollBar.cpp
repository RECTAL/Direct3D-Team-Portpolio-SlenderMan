#include "CSpriteObject_ScrollBar.h"
#include "CSpriteObject_Button.h"
#include "../../../Manager/CInputManager.h"
#include "../../../System/WindowSystem/WindowSystem_CWindowButton.h"
#include "../../../System/WindowSystem/WindowSystem_CWindowScrollBar.h"


CSpriteObject_ScrollBar::CSpriteObject_ScrollBar(std::string a_stSpriteTexturePath, std::string a_stExtantion, const float width, const float height, int a_nAnimationFrame)
	:CSpriteObject(a_stSpriteTexturePath, a_stExtantion, width, height, a_nAnimationFrame)
{
	m_ECWindowType = CWindowType::SCROLLBAR;
}



CSpriteObject_ScrollBar::~CSpriteObject_ScrollBar()
{
	this->release();
}

void CSpriteObject_ScrollBar::update()
{
	if (m_bIsVisible)
	{
		CSpriteObject::update();
		m_pWindow->update();
		setPosition(m_pWindow->getAbsolutePosition());
		m_pSpriteButton->getWindow()->setAbsolutePosition(m_pWindow->getAbsolutePosition() + m_stMoveOffset);
		if (PtInRect(&m_pWindow->getActiveRect(), GET_MOUSE_POSITION()))
		{
			if (IS_MOUSE_BUTTON_PRESSED(EMouseInput::LEFT))
			{
				if (m_fWidth > m_fHeight)
				{
					D3DXVECTOR3 stPos = D3DXVECTOR3(GET_MOUSE_POSITION().x, getPosition().y, 0.0f);
					m_pSpriteButton->getWindow()->setAbsolutePosition(stPos);
				}
				else
				{
					D3DXVECTOR3 stPos = D3DXVECTOR3(getPosition().x, GET_MOUSE_POSITION().y, 0.0f);
					m_pSpriteButton->getWindow()->setAbsolutePosition(stPos);
				}
			}
			else if (IS_MOUSE_BUTTON_DOWN(EMouseInput::LEFT))
			{
				if (m_fWidth > m_fHeight)
				{
					D3DXVECTOR3 stPos = D3DXVECTOR3(GET_MOUSE_POSITION().x, getPosition().y, 0.0f);
					m_pSpriteButton->getWindow()->setAbsolutePosition(stPos);
				}
				else
				{
					D3DXVECTOR3 stPos = D3DXVECTOR3(getPosition().x, GET_MOUSE_POSITION().y, 0.0f);
					m_pSpriteButton->getWindow()->setAbsolutePosition(stPos);
				}
			}
			if (IS_MOUSE_BUTTON_RELEASED(EMouseInput::LEFT))
			{

			}
		
		}
		calSetValue();
		m_pSpriteButton->update();
	}
}

void CSpriteObject_ScrollBar::doDrawUI()
{
	CSpriteObject::doDrawUI();
	m_pSpriteButton->doDrawUI();
}

void CSpriteObject_ScrollBar::init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc, float minVal, float maxVal, CSpriteObject_Button* a_pSpriteButton, bool a_bIsChild, D3DXVECTOR3 a_rstRelativePos)
{
	m_pWindow = new CWindowScrollBar("WindowScrollBar", CWindowType::SCROLLBAR, SIZE{ (LONG)m_fWidth,(LONG)m_fHeight}, getPosition());
	m_pWindow->init(a_pCrashCallBackFunc, a_pBeginCallBackFunc, a_pCallBackFunc, a_pEndCallBackFunc);
	m_bIsChild = a_bIsChild;
	m_stRelativePos = a_rstRelativePos;

	m_fMinVal = minVal;
	m_fMaxVal = maxVal;


	m_pSpriteButton = a_pSpriteButton;
	m_pSpriteButton->getWindow()->setAbsolutePosition(D3DXVECTOR3(getPosition().x, m_pWindow->getActiveRect().top, 0.0f));
	m_pSpriteButton->getRelativePos() = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (m_fWidth > m_fHeight)
	{
		float x = m_pSpriteButton->getWindow()->getAbsolutePosition().x;
		m_stMoveOffset = D3DXVECTOR3(x - m_pWindow->getAbsolutePosition().x + m_fMaxVal / 2, 0.0f, 0.0f);
	}
	else
	{
		float y = m_pSpriteButton->getWindow()->getAbsolutePosition().y;
		m_stMoveOffset = D3DXVECTOR3(0.0f, y - m_pWindow->getAbsolutePosition().y, 0.0f);
	}

}

void CSpriteObject_ScrollBar::release()
{
	CSpriteObject::release();
}

void CSpriteObject_ScrollBar::calSetValue()
{
	if (m_fWidth > m_fHeight)
	{
		float movePerVal = (m_fMaxVal - m_fMinVal) / m_fWidth;
		RECT rc = m_pWindow->getActiveRect();
		float x = m_pSpriteButton->getWindow()->getAbsolutePosition().x;
		m_stMoveOffset = D3DXVECTOR3(x - m_pWindow->getAbsolutePosition().x, 0.0f, 0.0f);
		m_fSetVal = m_fMinVal + (x - rc.left)*movePerVal;
		m_fSetVal = min(m_fSetVal, m_fMaxVal);
		m_fSetVal = max(m_fSetVal, m_fMinVal);
	}
	else
	{
		float movePerVal = (m_fMaxVal - m_fMinVal) / m_fHeight;
		RECT rc = m_pWindow->getActiveRect();
		float y = m_pSpriteButton->getWindow()->getAbsolutePosition().y;
		m_stMoveOffset = D3DXVECTOR3(0.0f, y - m_pWindow->getAbsolutePosition().y, 0.0f);
		m_fSetVal = m_fMinVal + (y - rc.top)*movePerVal;
		m_fSetVal = min(m_fSetVal, m_fMaxVal);
		m_fSetVal = max(m_fSetVal, m_fMinVal);
	}
}