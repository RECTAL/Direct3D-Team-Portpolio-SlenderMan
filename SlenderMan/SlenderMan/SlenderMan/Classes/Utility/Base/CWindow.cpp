#include "CWindow.h"
#include "../Object/SpriteObject/CSpriteObject.h"
CWindow::CWindow(std::string a_stWindowName, CWindowType a_ECWindowType, D3DXVECTOR3 a_stAbsolutePos)
	:m_stWindowName(a_stWindowName),m_eCWindowType(a_ECWindowType),m_stAbsolutePosition(a_stAbsolutePos)
{
}

CWindow::~CWindow()
{
	this->release();
}

void CWindow::update(void)
{
}


void CWindow::init(std::function<void(void)>* a_pCrashCallBackFunc, std::function<void(void)>* a_pBeginCallBackFunc, std::function<void(void)>* a_pCallBackFunc, std::function<void(void)>* a_pEndCallBackFunc)
{
}

void CWindow::release()
{

}
