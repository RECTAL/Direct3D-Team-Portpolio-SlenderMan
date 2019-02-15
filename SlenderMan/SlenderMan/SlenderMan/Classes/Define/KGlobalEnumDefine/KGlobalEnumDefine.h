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
	WALKGRASS,	// Ǯ ��� �Ҹ�
	WALKROCK,	// �� ��� �Ҹ�
	WALKREED,	// ���� ��� �Ҹ�
	PICK,		// ���� ȸ��
	SLENDER,	// �������� ����
	HEARTBEAT,	// �ɹڼ��� �ö�
	DIE,		// ���� ����
	NONE
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