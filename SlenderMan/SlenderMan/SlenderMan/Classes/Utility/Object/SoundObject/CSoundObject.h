#pragma once

#include "../../../Define/KGlobalDefine.h"

//! ����
class CSoundObject
{
public:			// setter

	//! ������ �����Ѵ�
	void setVolume(float a_fVolume);

public:			// public �Լ�

	//! ���� ���� ���θ� �˻��Ѵ�
	bool isLooping(void) const;

	//! ���� ��� ���θ� �˻��Ѵ�
	bool isPlaying(void) const;

	//! ���带 ����Ѵ�
	void playSound(bool a_bIsLoop);

	//! ���带 �����Ѵ�
	void stopSound(void);

public:			// ������, �Ҹ���

	//! ������
	CSoundObject(const std::string &a_rFilepath);

	//! �Ҹ���
	virtual ~CSoundObject(void);

private:			// private �Լ�

	//! ���� ���۸� �����Ѵ�
	LPDIRECTSOUNDBUFFER createSecondBuffer(void);

private:			// private ����

	STWaveSound m_stWaveSound;
	LPDIRECTSOUNDBUFFER m_pSecondBuffer = nullptr;
};
