//==========================================================
/// @file
/// @brief    �萔�w�b�_�[
/// @author   �������V
///
/// @attention  �Q�[�����̃p�����[�^��A�摜�E�����t�@�C�����Ȃǂ�萔�Ƃ��Ď����Ă��܂��B
///				�������̃}�N�������ɂ��Ă��L�q����Ă��܂��B

//==========================================================
#ifndef _CONSTANTS_H            // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _CONSTANTS_H            // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <windows.h>

//=============================================================================
//                  �萔
//=============================================================================

// �E�B���h�E�Ɋւ���萔
const char CLASS_NAME[] = "CastleDiffender";
const char GAME_TITLE[] = "CastleDiffender ver1.0";
const bool FULLSCREEN = false;							// �t���X�N���[���̏ꍇtrue
const UINT GAME_WIDTH = 1280;							// �Q�[����ʂ̕��i�s�N�Z���P�ʁj
const UINT GAME_HEIGHT = 720;							// �Q�[����ʂ̍����i�s�N�Z���P�ʁj

// �Q�[�����̒萔
const double PI = 3.14159265;							// �~����
const float FRAME_RATE = 60.0f;							// �t���[�����[�g (frames/sec)
const float MIN_FRAME_RATE = 10.0f;						// �ŏ��̃t���[�����[�g
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;			// 1�t���[���ɗv����ŏ��̎���
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;		// 1�t���[���ɗv����ő�̎���

// �Q�[�����̃p�����[�^
// �E�Ҋ֌W
const int BRAVE_ATTACK_DAMAGE = 50;					// �E�҂̍U���́i�_���[�W�ʁj
const int BRAVE_FIRE_DAMAGE = 110;					// �E�҂̃t�@�C�A�[�i�K�E�Z�j�̍U���́i�_���[�W�ʁj
const int BRAVE_RECOVERY_TO_BARRICADE = 20;			// �E�҂̍U���ł̃o���P�[�h�̉񕜗�
const int BRAVE_MP_RECOVERY = 10;					// �E�҂�MP�񕜗ʁimpRecoveryTime�b���Ƃɉ񕜁j
const float BRAVE_MP_RECOVERY_TIME = 2.0f;			// �E�҂�MP���񕜂���Ԋu�i�b�j
const float BRAVE_MOVE_SPEED = 150.0f;				// �E�҂̈ړ����x�i�s�N�Z���j
// �G���G�֌W
const float ENEMY_ATTACK_DAMAGE = 2.5f;					// �G���G�̍U���́i�v���C���[�Ə�ւ̃_���[�W�ʁj
const float ENEMY_ATTACK_DAMEGE_BARRICADE = 10.0f;		// �G���G�̍U���́i�o���P�[�h�ւ̃_���[�W�ʁj
const float ENEMY_MOVE_SPEED = 80.0f;					// �ړ����x�i�s�N�Z���j
// ���{�X�֌W
const float MIDBOSS_ATTACK_DAMAGE = 15.0f;				// ���{�X�̍U���́i�v���C���[�Ə�ւ̃_���[�W�ʁj
const float MIDBOSS_ATTACK_DAMEGE_BARRICADE = 20.0f;	// �G���G�̍U���́i�o���P�[�h�ւ̃_���[�W�ʁj

// �摜
const char MENU_IMAGE[] = "pictures\\systemGraphics\\title.png";				// ���j���[�摜
const char STAGE_SELECT_IMAGE[] = "pictures\\systemGraphics\\stageselect.png";	// �X�e�[�W�I����ʉ摜
const char RESULT_IMAGE[] = "pictures\\systemGraphics\\gameover.png";           // ���U���g�摜
const char STAGE_CLEAR_IMAGE[] = "pictures\\systemGraphics\\stageclear.png";	// �X�e�[�W�N���A�摜
const char DESCRIPTION_IMAGE[] = "pictures\\systemGraphics\\description.png";   // ��������摜
const char BRAVE_MOVE_IMAGE[] = "pictures\\alex\\alex_main.png";				// �E�҂̕��s�摜
const char FIRE_IMAGE[] = "pictures\\alex\\fire.png";							// ���̉摜
const char ENEMY_IMAGE[] = "pictures\\enemy\\enemy.png";						// �G���G�̉摜
const char MID_BOSS_IMAGE[] = "pictures\\enemy\\midBoss.png";					// ���{�X�̉摜
const char TILE_IMAGES[] = "pictures\\map\\ground.png";							// �}�b�v�摜
const char DASHBOARD_TEXTURES[] = "pictures\\systemGraphics\\dashboard.png";	// �_�b�V���{�[�h�摜
const char BARRICADE_IMAGE[] = "pictures\\map\\barricade.png";					// �o���P�[�h�̉摜
const char HIT_EFFECT_IMAGE[] = "pictures\\effect\\hitEffect.png";				// �_���[�W�G�t�F�N�g�̉摜
const char ATTACK_EFFECT_IMAGE[] = "pictures\\effect\\attackEffect.png";		// �U���G�t�F�N�g�̉摜
const char COLLISION_IMAGE[] = "pictures\\alex\\attackCollision.png";			// �U���p�̃e�N�X�`��
const char CASTLE_IMAGE[] = "pictures\\map\\castle.png";						// ��摜
const char BRAVE_ICON_IMAGE[] = "pictures\\alex\\icon.png";						// �E�҂̃A�C�R���摜
const char TEXT_IMAGE[] = "pictures\\systemGraphics\\text.png";					// �e�L�X�g�i�l�o�A�g�o�j�摜
const char CASTLE_ICON_IMAGE[] = "pictures\\map\\castleIcon.png";				// ��̃A�C�R���̉摜
const char FONT_IMAGE[] = "pictures\\systemGraphics\\CKfont.png";				// �X�v���C�g�e�L�X�g�̉摜

// ����
// WAVE_BANK�ixwb�t�@�C���j
const char WAVE_BANK[] = "audio\\Win\\Wave Bank.xwb";
// SOUND_BANK�ixsb�t�@�C���j
const char SOUND_BANK[] = "audio\\Win\\Sound Bank.xsb";
// �I�[�f�B�I�L���[
const char TITLE[]    = "title";	// �^�C�g����
const char STAGE[]    = "stage";	// �X�e�[�W��
const char CLEAR[]    = "clear";	// �N���A��
const char GAMEOVER[] = "gameover";	// �Q�[���I�[�o�[��
const char KILL[]     = "kill";		// �v���C���[�U����
const char FIRE_SE[]  = "fire";		// �v���C���[�K�E�Z��


// �L�[�}�b�v
const UCHAR CONSOLE_KEY = '`';				// `�L�[
const UCHAR ESC_KEY = VK_ESCAPE;			// Esc�L�[
const UCHAR ALT_KEY = VK_MENU;				// Alt�L�[
const UCHAR ENTER_KEY = VK_RETURN;			// Enter�L�[
const UCHAR BRAVE_LEFT_KEY = VK_LEFT;		// ���ړ��L�[
const UCHAR BRAVE_RIGHT_KEY = VK_RIGHT;		// �E�ړ��L�[
const UCHAR BRAVE_UP_KEY = VK_UP;			// ��ړ��L�[
const UCHAR BRAVE_DOWN_KEY = VK_DOWN;		// ���ړ��L�[
const UCHAR BRAVE_ATTACK_KEY = 'A';			// �U���L�[
const UCHAR BRAVE_GAURD_KEY = 'D';			// �K�[�h�L�[
const UCHAR BRAVE_FIRE_KEY = 'S';			// �K�E�Z�L�[

// WEAPON�̎�ށi���A�E�҂̍U���A�E�҂̑�񌂁A�G���G�̍U���A���{�X�̍U���j
enum WEAPON { FIRE, BRAVE_ATTACK, BRAVE_SECOND_ATTACK, ENEMY_ATTACK , MIDBOSS_ATTACK};

//=============================================================================
// �}�N������
//=============================================================================
// �|�C���^�Q�Ƃ����A�C�e�������S�ɉ��
template <typename T>
inline void safeRelease(T& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#define SAFE_RELEASE safeRelease            // ���ʌ݊������ێ����邽��

// �|�C���^�Q�Ƃ����A�C�e�������S�ɍ폜
template <typename T>
inline void safeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE safeDelete              // ���ʌ݊������ێ����邽��

// �|�C���^�Q�Ƃ����z������S�ɍ폜
template <typename T>
inline void safeDeleteArray(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // ���ʌ݊������ێ����邽��

// onLostDevice�����S�ɌĂяo��
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
	if (ptr)
		ptr->onLostDevice();
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // ���ʌ݊������ێ����邽��

// onResetDevice�����S�ɌĂяo��
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
	if (ptr)
		ptr->onResetDevice();
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // ���ʌ݊������ێ����邽��

#endif
