#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CUIObject.h"

//! 라벨
class CLabelObject : public CUIObject
{
public:			// setter

	//! 문자열을 변경한다
	void setString(const std::string &a_rString);

public:			// 생성자, 소멸자

	//! 생성자
	CLabelObject(const std::string &a_rString, float a_fSize);

	//! 소멸자
	virtual ~CLabelObject(void);

protected:			// protected 함수

	//! UI 를 그린다
	virtual void doDrawUI(void) override;

private:			// private 함수

	//! 폰트를 생성한다
	LPD3DXFONT createFont(float a_fSize);

private:			// private 변수

	std::string m_oString = "";
	LPD3DXFONT m_pFont = nullptr;
};
