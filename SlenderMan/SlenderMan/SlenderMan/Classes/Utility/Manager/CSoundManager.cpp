#include "CSoundManager.h"
#include "CWindowManager.h"
#include "../Object/SoundObject/CSoundObject.h"

CSoundManager::CSoundManager(void)
	:
	m_fEffectSoundsVolume(1.0f),
	m_fBackgroundSoundVolume(1.0f)
{
	// Do Nothing
}

CSoundManager::~CSoundManager(void)
{
	for (auto &rValueType : m_oEffectSoundContainer) {
		for (auto pSound : rValueType.second) {
			SAFE_DELETE(pSound);
		}
	}

	SAFE_DELETE(m_pBackgroundSound);

	SAFE_RELEASE(m_pPrimaryBuffer);
	SAFE_RELEASE(m_pDirectSound);
}

void CSoundManager::init(void)
{
	m_pDirectSound = this->createDirectSound();
	m_pPrimaryBuffer = this->createPrimaryBuffer();
}

LPDIRECTSOUND CSoundManager::getDirectSound(void) const
{
	return m_pDirectSound;
}

LPDIRECTSOUNDBUFFER CSoundManager::getPrimaryBuffer(void) const
{
	return m_pPrimaryBuffer;
}

void CSoundManager::setEffectSoundsVolume(float a_fVolume)
{
	m_fEffectSoundsVolume = a_fVolume;

	for (auto &rValueType : m_oEffectSoundContainer) {
		for (auto pSound : rValueType.second) {
			if (pSound != nullptr) {
				pSound->setVolume(a_fVolume);
			}
		}
	}
}

void CSoundManager::setBackgroundSoundVolume(float a_fVolume)
{
	m_fBackgroundSoundVolume = a_fVolume;

	if (m_pBackgroundSound != nullptr) {
		m_pBackgroundSound->setVolume(a_fVolume);
	}
}

CSoundObject * CSoundManager::playEffectSound(const std::string & a_rFilepath, bool a_bIsLoop)
{
	auto pSound = this->findPlayableEffectSound(a_rFilepath);

	if (pSound != nullptr) {
		pSound->playSound(a_bIsLoop);
		this->setEffectSoundsVolume(m_fEffectSoundsVolume);
	}

	return pSound;
}

CSoundObject * CSoundManager::playBackgroundSound(const std::string & a_rFilepath, bool a_bIsLoop)
{
	if (m_pBackgroundSound != nullptr) {
		m_pBackgroundSound->stopSound();
		SAFE_DELETE(m_pBackgroundSound);
	}

	m_pBackgroundSound = new CSoundObject(a_rFilepath);
	m_pBackgroundSound->playSound(a_bIsLoop);

	this->setBackgroundSoundVolume(m_fBackgroundSoundVolume);
	return m_pBackgroundSound;
}

void CSoundManager::setEffectSoundVolume(const std::string & a_rFilepath, float a_fVolume)
{
	if (m_oEffectSoundContainer.find(a_rFilepath) != m_oEffectSoundContainer.end()) {
		auto oIterator = m_oEffectSoundContainer.find(a_rFilepath);
		for (auto iter : oIterator->second)
		{
			iter->setVolume(a_fVolume);
		}
	}
}

void CSoundManager::stopAllEffectSounds(void)
{
	for (auto &rValueType : m_oEffectSoundContainer) {
		for (auto pSound : rValueType.second) {
			if (pSound != nullptr) {
				pSound->stopSound();
			}
		}
	}
}

void CSoundManager::stopBackgroundSound(void)
{
	if (m_pBackgroundSound != nullptr) {
		m_pBackgroundSound->stopSound();
	}
}

CSoundObject * CSoundManager::findPlayableEffectSound(const std::string & a_rFilepath)
{
	// 효과음 리스트가 없을 경우
	if (m_oEffectSoundContainer.find(a_rFilepath) == m_oEffectSoundContainer.end()) {
		TEffectSoundList oEffectSoundList{
			nullptr
		};

		m_oEffectSoundContainer.insert(decltype(m_oEffectSoundContainer)::value_type(a_rFilepath,
			oEffectSoundList));
	}

	auto oIterator = m_oEffectSoundContainer.find(a_rFilepath);

	// 사운드를 탐색한다
	auto oSoundIterator = std::find_if(oIterator->second.begin(),
		oIterator->second.end(),
		[=](CSoundObject *a_pSound) -> bool
	{
		return a_pSound == nullptr || !a_pSound->isPlaying();
	});

	// 재생 가능한 사운드가 있을 경우
	if (oSoundIterator != oIterator->second.end()) {
		// 생성 된 사운드가 있을 경우
		if (*oSoundIterator != nullptr) {
			return *oSoundIterator;
		}

		auto pSound = new CSoundObject(a_rFilepath);
		int nIndex = oSoundIterator - oIterator->second.begin();

		oIterator->second[nIndex] = pSound;
		return pSound;
	}

	return nullptr;
}

LPDIRECTSOUND CSoundManager::createDirectSound(void)
{
	LPDIRECTSOUND pDirectSound = nullptr;
	DirectSoundCreate(NULL, &pDirectSound, NULL);

	pDirectSound->SetCooperativeLevel(GET_WINDOW_HANDLE(), DSSCL_PRIORITY);
	return pDirectSound;
}

LPDIRECTSOUNDBUFFER CSoundManager::createPrimaryBuffer(void)
{
	DSBUFFERDESC stBufferDesc;
	ZeroMemory(&stBufferDesc, sizeof(stBufferDesc));

	/*
	주 버퍼는 직접적으로 생성하는 것이 불가능하기 때문에
	DSBUFFERDESC.dwFlags 에 DSCAPS_PRIMARYBUFFER 옵션을 설정하는 것은
	내부적으로 버퍼의 생성이 아니라 주 버퍼에 대한 포인터를 얻어온다는 것을
	의미한다.
	*/
	stBufferDesc.dwSize = sizeof(stBufferDesc);
	stBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;

	LPDIRECTSOUNDBUFFER pSoundBuffer = nullptr;
	m_pDirectSound->CreateSoundBuffer(&stBufferDesc, &pSoundBuffer, NULL);

	// 버퍼 포맷을 설정한다
	// {
	/*
	WAVEFORMATEX 구조체 맴버
	:
	- nChannels (사운드가 재생 되는 채널 수)
	- nSamplesPerSec (초당 샘플링 횟수)
	- nAvgBytesPerSec (초당 샘플링 되는 바이트 수)
	- nBlockAlign (사운드 데이터 한 블럭에 대한 바이트 수)
	- wBitsPerSample (샘플링 되는 데이터의 비트 수)
	*/
	WAVEFORMATEX stWaveFormat;
	ZeroMemory(&stWaveFormat, sizeof(stWaveFormat));

	stWaveFormat.cbSize = sizeof(stWaveFormat);
	stWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
	stWaveFormat.nChannels = 2;
	stWaveFormat.nSamplesPerSec = 22050;
	stWaveFormat.wBitsPerSample = 16;
	stWaveFormat.nBlockAlign = (stWaveFormat.wBitsPerSample / 8) * stWaveFormat.nChannels;
	stWaveFormat.nAvgBytesPerSec = stWaveFormat.nBlockAlign * stWaveFormat.nSamplesPerSec;

	pSoundBuffer->SetFormat(&stWaveFormat);
	// }

	return pSoundBuffer;
}
