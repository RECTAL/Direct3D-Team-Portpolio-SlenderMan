#include "CSoundObject.h"
#include "../../Manager/CSoundManager.h"
#include "../../Manager/CResourceManager.h"

CSoundObject::CSoundObject(const std::string & a_rFilepath)
{
	m_stWaveSound = GET_WAVE_SOUND(a_rFilepath);
	m_pSecondBuffer = this->createSecondBuffer();
}

CSoundObject::~CSoundObject(void)
{
	SAFE_RELEASE(m_pSecondBuffer);
}

void CSoundObject::setVolume(float a_fVolume)
{
	float fVolume = min(1.0f, a_fVolume);
	fVolume = max(0.0f, fVolume);

	m_pSecondBuffer->SetVolume((1.0f - fVolume) * DSBVOLUME_MIN);
}

bool CSoundObject::isLooping(void) const
{
	DWORD nStatus = 0;
	m_pSecondBuffer->GetStatus(&nStatus);

	return nStatus & DSBSTATUS_LOOPING;
}

bool CSoundObject::isPlaying(void) const
{
	DWORD nStatus = 0;
	m_pSecondBuffer->GetStatus(&nStatus);

	return nStatus & DSBSTATUS_PLAYING;
}

void CSoundObject::playSound(bool a_bIsLoop)
{
	DWORD nFlags = a_bIsLoop ? DSBPLAY_LOOPING : 0;

	m_pSecondBuffer->SetCurrentPosition(0);
	m_pSecondBuffer->Play(0, 0, nFlags);
}

void CSoundObject::stopSound(void)
{
	m_pSecondBuffer->Stop();
}

LPDIRECTSOUNDBUFFER CSoundObject::createSecondBuffer(void)
{
	DSBUFFERDESC stBufferDesc;
	ZeroMemory(&stBufferDesc, sizeof(stBufferDesc));

	stBufferDesc.dwSize = sizeof(stBufferDesc);
	stBufferDesc.dwBufferBytes = m_stWaveSound.m_nNumBytes;
	stBufferDesc.guid3DAlgorithm = GUID_NULL;
	stBufferDesc.lpwfxFormat = &m_stWaveSound.m_stWaveFormat;
	stBufferDesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME;

	LPDIRECTSOUNDBUFFER pSoundBuffer = nullptr;
	GET_DIRECT_SOUND()->CreateSoundBuffer(&stBufferDesc, &pSoundBuffer, NULL);

	// 사운드 정보를 설정한다
	// {
	BYTE *pnBytesA = nullptr;
	BYTE *pnBytesB = nullptr;

	DWORD nNumBytesA = 0;
	DWORD nNumBytesB = 0;

	if (SUCCEEDED(pSoundBuffer->Lock(0,
		m_stWaveSound.m_nNumBytes,
		(void **)&pnBytesA,
		&nNumBytesA,
		(void **)&pnBytesB,
		&nNumBytesB,
		0)))
	{
		CopyMemory(pnBytesA, m_stWaveSound.m_pnBytes, m_stWaveSound.m_nNumBytes);
		pSoundBuffer->Unlock(NULL, 0, NULL, 0);
	}
	// }

	return pSoundBuffer;
}
