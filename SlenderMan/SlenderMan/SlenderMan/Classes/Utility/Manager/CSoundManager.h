#pragma once

#include "../../Define/KGlobalDefine.h"

class CSoundObject;

//! Sound Manager
class CSoundManager
{
public:

	enum
	{
		MAX_NUM_DUPLICATE_EFFECT_SOUNDS = 1
	};

	
	typedef std::array<CSoundObject *, MAX_NUM_DUPLICATE_EFFECT_SOUNDS> TEffectSoundList;
	typedef std::unordered_map<std::string, TEffectSoundList> TEffectSoundContainer;

public:			// getter, setter

	//! 다이렉트 사운드를 반환한다
	LPDIRECTSOUND getDirectSound(void) const;

	//! 주 버퍼를 반환한다
	LPDIRECTSOUNDBUFFER getPrimaryBuffer(void) const;

	//! 효과음 볼륨을 변경한다
	void setEffectSoundsVolume(float a_fVolume);

	//! 배경음 볼륨을 변경한다
	void setBackgroundSoundVolume(float a_fVolume);


public:			// public 함수

	//! 싱글턴
	DECLARE_SINGLETON(CSoundManager);

	//! 초기화
	void init(void);

	//! 효과음을 재생한다
	CSoundObject * playEffectSound(const std::string &a_rFilepath, bool a_bIsLoop = false);

	//! 배경음을 재생한다
	CSoundObject * playBackgroundSound(const std::string &a_rFilepath, bool a_bIsLoop = true);


	void setEffectSoundVolume(const std::string &a_rFilepath, float a_fVolume);

	//! 효과음을 중지한다
	void stopAllEffectSounds(void);

	//! 배경음을 중지한다
	void stopBackgroundSound(void);

private:			// private 함수

	//! 재생 가능한 효과음을 탐색한다
	CSoundObject * findPlayableEffectSound(const std::string &a_rFilepath);

	//! 다이렉트 사운드를 생성한다
	LPDIRECTSOUND createDirectSound(void);

	//! 주 버퍼를 생성한다
	LPDIRECTSOUNDBUFFER createPrimaryBuffer(void);

private:			// 생성자, 소멸자

	//! 생성자
	CSoundManager(void);

	//! 소멸자
	virtual ~CSoundManager(void);

private:			// private 변수

	float m_fEffectSoundsVolume = 0.0f;
	float m_fBackgroundSoundVolume = 0.0f;

	LPDIRECTSOUND m_pDirectSound = nullptr;
	LPDIRECTSOUNDBUFFER m_pPrimaryBuffer = nullptr;

	CSoundObject *m_pBackgroundSound = nullptr;
	TEffectSoundContainer m_oEffectSoundContainer;
};
