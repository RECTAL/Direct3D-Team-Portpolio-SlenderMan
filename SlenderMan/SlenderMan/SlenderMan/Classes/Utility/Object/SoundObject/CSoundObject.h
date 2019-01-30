#pragma once

#include "../../../Define/KGlobalDefine.h"

//! 사운드
class CSoundObject
{
public:			// setter

	//! 볼륨을 변경한다
	void setVolume(float a_fVolume);

public:			// public 함수

	//! 사운드 루프 여부를 검사한다
	bool isLooping(void) const;

	//! 사운드 재생 여부를 검사한다
	bool isPlaying(void) const;

	//! 사운드를 재생한다
	void playSound(bool a_bIsLoop);

	//! 사운드를 중지한다
	void stopSound(void);

public:			// 생성자, 소멸자

	//! 생성자
	CSoundObject(const std::string &a_rFilepath);

	//! 소멸자
	virtual ~CSoundObject(void);

private:			// private 함수

	//! 이차 버퍼를 생성한다
	LPDIRECTSOUNDBUFFER createSecondBuffer(void);

private:			// private 변수

	STWaveSound m_stWaveSound;
	LPDIRECTSOUNDBUFFER m_pSecondBuffer = nullptr;
};
