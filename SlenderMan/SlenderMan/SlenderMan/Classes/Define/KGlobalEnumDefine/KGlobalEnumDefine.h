#pragma once

enum class EDirection
{
	LEFT,
	RIGHT,
	FRONT,
	BACK
};

//! MouseInput Enum Class
enum class EMouseInput
{
	LEFT,
	RIGHT,
	MIDDLE,
	NONE
};

//! ParticleType Enum Class
enum class EParticleType
{
	NONE
};

//! 윈도우 종류
enum class CWindowType
{
	CONTAINER,
	BUTTON,
	EDITER,
	LIST,
	SCROLLBAR,
	NONE
};

enum class EObjClasses
{
	SKINNED,STATIC,DECORATE_SOUND,DECORATE_BILLBOARD,NONE
};

enum class EObjType
{
	DESK,
	GRASS,
	MOUNTAIN,
	OLDHOUSE,
	OLDWOODDOCK,
	PLANTS,
	ROUNDWOOD,
	LOG,
	WOODHOUSE,
	TREE_1,
	TREE_2,
	TREE_3,
	TREE_4,
	TREE_5,
	TREE_6,
	TREE_7,
	TREE_8,
	ROCK_0,
	ROCK_1,
	WIND,
	RAIN,
	CRIKET,
	CROW,
	FIRE,
	OWL,
	PAGE_1,
	PAGE_2,
	PAGE_3,
	PAGE_4,
	PAGE_5,
	PAGE_6,
	PAGE_7,
	PAGE_8,
	NONE = -1
};

enum class EPlayerState
{
	WALKGRASS = 0x0001,	// 풀 밟는 소리
	WALKROCK = 0x0002,	// 돌 밟는 소리
	WALKREED = 0x0004,	// 갈대 밟는 소리
	PICK = 0x0008,		// 쪽지 회수
	SLENDER = 0x0010,	// 슬랜더맨 만남
	HEARTBEAT = 0x0020,	// 심박수가 올라간
	DIE = 0x0040,		// 죽은 상태
	RUN = 0x0080,		// 달리는 상태
	NONE = 0x0000
};

enum class EPlayingBGM
{
	WIND,			// 바람 소리
	RAIN,			// 빗 소리
	CRIKET,			// 귀뚜라미 소리
	CROW,			// 까마귀 소리
	FIRE,			// 불 소리
	OWL,			// 부엉이 소리
	NOISE_1,		// 약한 노이즈
	NOISE_2,		// 강한 노이즈
	NOISE_3,		// 사망 노이즈
	NONE
};

enum class EStageSound
{
	START,			// 시작 BGM
	STAGE_1,		// 중간 BGM
	STAGE_2,		// 최종 BGM
	NONE
};

//! 디버그 타입
enum class EDebugDrawType
{
	BOX,
	SPHERE,
	NONE
};