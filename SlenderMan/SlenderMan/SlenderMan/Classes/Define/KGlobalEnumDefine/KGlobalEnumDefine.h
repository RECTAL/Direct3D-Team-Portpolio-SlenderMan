#pragma once


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

enum class EObjType
{
	DESK,
	GRASS,
	MOUNTAIN,
	OLDHOUSE,
	OLDWOODDOCK,
	PLANTS,
	ROUNDWOOD,
	URBANDEBRIS,
	WOODHOUSE,
	TREE_1,
	TREE_2,
	TREE_3,
	TREE_4,
	TREE_5,
	TREE_6,
	ROCK_0,
	ROCK_1,
	WIND,
	RAIN,
	CRIKET,
	CROW,
	FIRE,
	OWL,
	NONE = -1
};

enum class EPlayerState
{
	WALKGRASS,	// 풀 밟는 소리
	WALKROCK,	// 돌 밟는 소리
	WALKREED,	// 갈대 밟는 소리
	PICK,		// 쪽지 회수
	SLENDER,	// 슬랜더맨 만남
	HEARTBEAT,	// 심박수가 올라간
	DIE,		// 죽은 상태
	NONE
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
	START,			// 시작 지점
	STAGE_1,		// 첫번째 쪽지 근처
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
	STAGE_6,
	STAGE_7,
	STAGE_8,
	EXIT,			// 탈출 시
	NONE
};


//! 디버그 타입
enum class EDebugDrawType
{
	BOX,
	SPHERE,
	NONE
};