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

//! ������ ����
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
	SKINNED,STATIC,DECORATE_SOUND,NONE
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
	WALKGRASS = 0x0001,	// Ǯ ��� �Ҹ�
	WALKROCK = 0x0002,	// �� ��� �Ҹ�
	WALKREED = 0x0004,	// ���� ��� �Ҹ�
	PICK = 0x0008,		// ���� ȸ��
	SLENDER = 0x0010,	// �������� ����
	HEARTBEAT = 0x0020,	// �ɹڼ��� �ö�
	DIE = 0x0040,		// ���� ����
	RUN = 0x0080,		// �޸��� ����
	NONE = 0x0000
};

enum class EPlayingBGM
{
	WIND,			// �ٶ� �Ҹ�
	RAIN,			// �� �Ҹ�
	CRIKET,			// �ͶѶ�� �Ҹ�
	CROW,			// ��� �Ҹ�
	FIRE,			// �� �Ҹ�
	OWL,			// �ξ��� �Ҹ�
	NOISE_1,		// ���� ������
	NOISE_2,		// ���� ������
	NOISE_3,		// ��� ������
	NONE
};

enum class EStageSound
{
	START,			// ���� ����
	STAGE_1,		// ù��° ���� ��ó
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
	STAGE_6,
	STAGE_7,
	STAGE_8,
	EXIT,			// Ż�� ��
	NONE
};

//! ����� Ÿ��
enum class EDebugDrawType
{
	BOX,
	SPHERE,
	NONE
};