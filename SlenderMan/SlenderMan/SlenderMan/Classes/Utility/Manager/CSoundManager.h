#pragma once

#include "../../Define/KGlobalDefine.h"

class CSoundObject;

//! Sound Manager
class CSoundManager
{
public:

	enum
	{
		MAX_NUM_DUPLICATE_EFFECT_SOUNDS = 5
	};

	
	typedef std::array<CSoundObject *, MAX_NUM_DUPLICATE_EFFECT_SOUNDS> TEffectSoundList;
	typedef std::unordered_map<std::string, TEffectSoundList> TEffectSoundContainer;

public:			// getter, setter

	//! ���̷�Ʈ ���带 ��ȯ�Ѵ�
	LPDIRECTSOUND getDirectSound(void) const;

	//! �� ���۸� ��ȯ�Ѵ�
	LPDIRECTSOUNDBUFFER getPrimaryBuffer(void) const;

	//! ȿ���� ������ �����Ѵ�
	void setEffectSoundsVolume(float a_fVolume);

	//! ����� ������ �����Ѵ�
	void setBackgroundSoundVolume(float a_fVolume);

public:			// public �Լ�

	//! �̱���
	DECLARE_SINGLETON(CSoundManager);

	//! �ʱ�ȭ
	void init(void);

	//! ȿ������ ����Ѵ�
	CSoundObject * playEffectSound(const std::string &a_rFilepath, bool a_bIsLoop = false);

	//! ������� ����Ѵ�
	CSoundObject * playBackgroundSound(const std::string &a_rFilepath, bool a_bIsLoop = true);

	//! ȿ������ �����Ѵ�
	void stopAllEffectSounds(void);

	//! ������� �����Ѵ�
	void stopBackgroundSound(void);

private:			// private �Լ�

	//! ��� ������ ȿ������ Ž���Ѵ�
	CSoundObject * findPlayableEffectSound(const std::string &a_rFilepath);

	//! ���̷�Ʈ ���带 �����Ѵ�
	LPDIRECTSOUND createDirectSound(void);

	//! �� ���۸� �����Ѵ�
	LPDIRECTSOUNDBUFFER createPrimaryBuffer(void);

private:			// ������, �Ҹ���

	//! ������
	CSoundManager(void);

	//! �Ҹ���
	virtual ~CSoundManager(void);

private:			// private ����

	float m_fEffectSoundsVolume = 0.0f;
	float m_fBackgroundSoundVolume = 0.0f;

	LPDIRECTSOUND m_pDirectSound = nullptr;
	LPDIRECTSOUNDBUFFER m_pPrimaryBuffer = nullptr;

	CSoundObject *m_pBackgroundSound = nullptr;
	TEffectSoundContainer m_oEffectSoundContainer;
};
