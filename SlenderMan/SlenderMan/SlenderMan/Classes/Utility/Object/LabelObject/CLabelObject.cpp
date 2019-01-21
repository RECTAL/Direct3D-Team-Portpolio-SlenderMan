#include "CLabelObject.h"
#include "../../Manager/CDeviceManager.h"

CLabelObject::CLabelObject(const std::string & a_rString, float a_fSize)
	:	m_oString(a_rString)
{
	m_pFont = this->createFont(a_fSize);
}

CLabelObject::~CLabelObject(void)
{
	SAFE_RELEASE(m_pFont);
}

void CLabelObject::setString(const std::string & a_rString)
{
	m_oString = a_rString;
}

void CLabelObject::doDrawUI(void)
{
	RECT stTextRect = {
		this->getPosition().x, this->getPosition().y, 0, 0
	};

	// 크기를 계산한다
	m_pFont->DrawTextA(NULL,
		m_oString.c_str(),
		-1,
		&stTextRect,
		DT_TOP | DT_LEFT | DT_CALCRECT,
		m_nDiffuseColor);

	// 텍스트를 그린다
	m_pFont->DrawTextA(NULL,
		m_oString.c_str(),
		-1,
		&stTextRect,
		DT_TOP | DT_LEFT,
		m_nDiffuseColor);
}

LPD3DXFONT CLabelObject::createFont(float a_fSize)
{
	D3DXFONT_DESC stFontDesc;
	ZeroMemory(&stFontDesc, sizeof(stFontDesc));

	stFontDesc.Height = a_fSize;
	stFontDesc.Weight = 1000;

	LPD3DXFONT pFont = nullptr;

	// 폰트를 생성한다
	D3DXCreateFontIndirect(GET_DEVICE(),
		&stFontDesc,
		&pFont);

	return pFont;
}
