#pragma once

#include "../../../Define/KGlobalDefine.h"
#include "../../Base/CUIObject.h"

//! ��
class CLabelObject : public CUIObject
{
public:			// setter

	//! ���ڿ��� �����Ѵ�
	void setString(const std::string &a_rString);

public:			// ������, �Ҹ���

	//! ������
	CLabelObject(const std::string &a_rString, float a_fSize);

	//! �Ҹ���
	virtual ~CLabelObject(void);

protected:			// protected �Լ�

	//! UI �� �׸���
	virtual void doDrawUI(void) override;

private:			// private �Լ�

	//! ��Ʈ�� �����Ѵ�
	LPD3DXFONT createFont(float a_fSize);

private:			// private ����

	std::string m_oString = "";
	LPD3DXFONT m_pFont = nullptr;
};
