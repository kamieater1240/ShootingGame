//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <Windows.h>

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLEBGM = 0,		// Title BGM
	SOUND_LABEL_STAGEBGM,			// Stage BGM
	SOUND_LABEL_BOSSBGM,			// Boss BGM
	SOUND_LABEL_BOSSLUNABGM,		// Boss Lunatic BGM
	SOUND_LABEL_SE_PSHOT,			// �v���C���[�e���ˉ�
	SOUND_LABEL_SE_ESHOT,			// �G�l�~�[�e���ˉ�
	SOUND_LABEL_SE_EXPLOSION,		// ������
	SOUND_LABEL_SE_OK,				// OK��
	SOUND_LABEL_SE_SELECT,			// �I����
	SOUND_LABEL_SE_CANCEL,			// �L�����Z����
	SOUND_LABEL_SE_GETITEM,			// Get Item
	SOUND_LABEL_SE_DEATH,			// ���S��
	SOUND_LABEL_SE_BOSSDEATH,		// Boss ���S��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
