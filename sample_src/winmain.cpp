//==========================================================
/// @file
/// @brief    ���C���N���X
/// @author   �������V
///
/// @attention  ���C�����[�v�����s����N���X�ł��B
///				�ł���̊K�w�̃N���X�ŁAGame���p�������I�u�W�F�N�g�͂��̃N���X���Ǘ����܂��B

#define _CRTDBG_MAP_ALLOC	// ���������[�N�����o���邽��
#define WIN32_LEAN_AND_MEAN

#include "windows.h"
#include <stdlib.h>			// ���������[�N�����o���邽��
#include <crtdbg.h>			// ���������[�N�����o���邽��
#include "castleDiffender.h"

// �֐��v���g�^�C�v
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);
bool AnotherInstance();

// Game�|�C���^
CastleDiffender *game = NULL;
HWND hwnd = NULL;

//==========================================================
// Windows�A�v���P�[�V�����̊J�n�_
// �p�����[�^
//		hInstance�F�A�v���P�[�V�����̌��݂̃C���X�^���X�ւ̃n���h��
//		hPrevInstance�F���NULL�i�p�~���ꂽ�p�����[�^�j
//		lpCmdLine�F�R�}���h���C��������NULL�I�[�����ւ̃|�C���^
//		nCmdShow�F�E�B���h�E���ǂ̂悤�ɕ\�����邩���w��
//==========================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �f�o�b�O�r���h�̏ꍇ�A���������[�N���`�F�b�N
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	MSG msg;

	// �Q�[���𐶐��A���b�Z�[�W�n���h�����Z�b�g
	game = new CastleDiffender;
	
	// �����̃C���X�^���X�̐�����h��
	if (AnotherInstance())
		return false;

	nCmdShow = SW_MAXIMIZE;	// �E�B���h�E���ő剻���ĕ\������̂��f�t�H���g
	// �E�B���h�E���쐬
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
		return 1;

	try {
		game->initialize(hwnd);	// GameError���X���[

		// ���C���̃��b�Z�[�W���[�v
		int done = 0;
		while (!done)
		{
			// PeekMessage��Windows���b�Z�[�W�����邩���e�X�g�����u���b�N���\�b�h
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// �I�����b�Z�[�W�����m
				if (msg.message == WM_QUIT)
					done = 1;
				// ���b�Z�[�W���f�R�[�h����WinProc�ɓn��
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				game->run(hwnd);	// �Q�[�����[�v�����s
		}
		SAFE_DELETE(game);	// �I���̑O�Ƀ��������J��
		return msg.wParam;
	}
	catch (const GameError &err)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	catch (...)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occured in game", "Error", MB_OK);
	}
	SAFE_DELETE(game);	// �I���̑O�Ƀ��������J��
	return 0;
}

// �E�B���h�E�C�x���g�R�[���o�b�N�֐�
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return (game->messageHandler(hWnd, msg, wParam, lParam));
}

// �E�B���h�E���쐬
// �߂�l�F�G���[�̏ꍇ�Afalse
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;

	// �E�B���h�E�N���X�̍\���̂����C���E�B���h�E���L�q����p�����[�^�Őݒ肵�܂��B
	wcx.cbSize = sizeof(wcx);					// �\���̂̃T�C�Y
	wcx.style = CS_HREDRAW | CS_VREDRAW;		// �E�B���h�E�T�C�Y�ύX���ɍĕ`��
	wcx.lpfnWndProc = WinProc;					// �E�B���h�E�v���V�[�W�����w��
	wcx.cbClsExtra = 0;							// �g���N���X�������Ȃ�
	wcx.cbWndExtra = 0;							// �g���E�B���h�E�������Ȃ�
	wcx.hInstance = hInstance;					// �C���X�^���X�ւ̃n���h��
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);	// ���O��`����Ă�����J�[�\��
	wcx.hbrBackground =							// �w�i�u���V
		(HBRUSH)GetStockObject(BLACK_BRUSH);
	wcx.lpszMenuName = NULL;					// ���j���[���\�[�X�̖��O
	wcx.lpszClassName = CLASS_NAME;				// �E�B���h�E�N���X�̖��O
	wcx.hIconSm = NULL;

	// �E�B���h�E�N���X��o�^
	// �G���[���ARegisterClassEx��0��߂�
	if (RegisterClassEx(&wcx) == 0)				// �G���[�̏ꍇ
		return false;

	// ��ʂ��E�B���h�E�\�����[�h�ƑS��ʕ\�����[�h�̂ǂ���Őݒ肷�邩�H
	DWORD style;
	if (FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	// �E�B���h�E���쐬
	hwnd = CreateWindow(
		CLASS_NAME,				// �E�B���h�E�N���X�̖��O
		GAME_TITLE,				// �^�C�g���o�[�̃e�L�X�g
		style,					// �E�B���h�E�̃X�^�C��
		CW_USEDEFAULT,			// �E�B���h�E�̐����ʒu�̃f�t�H���g
		CW_USEDEFAULT,			// �E�B���h�E�̐����ʒu�̃f�t�H���g
		GAME_WIDTH,				// �E�B���h�E�̕�
		GAME_HEIGHT,			// �E�B���h�E�̍���
		(HWND)NULL,				// �e�E�B���h�E�Ȃ�
		(HMENU)NULL,			// ���j���[�Ȃ�
		hInstance,				// �A�v���P�[�V�����C���X�^���X�ւ̃n���h��
		(LPVOID)NULL);			// �E�B���h�E�p�����[�^�Ȃ�

	// �E�B���h�E�̍쐬�ŃG���[�����������ꍇ
	if (!hwnd)
		return false;
	if (!FULLSCREEN)	// �E�B���h�E�\���̏ꍇ
	{
		// �N���C�A���g�̈悪GAME_WIDTH * GAME_HEIGHT�ƂȂ�悤��
		// �E�B���h�E�T�C�Y�𒲐�
		RECT clientRect;
		// �E�B���h�E�̃N���C�A���g�̈�̃T�C�Y���m��
		GetClientRect(hwnd, &clientRect);
		MoveWindow(hwnd,
			0,													// ��
			0,													// ��
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),		// �E
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom),	// ��
			TRUE);												// �E�B���h�E���ĕ`��
	}
	// �E�B���h�E��\��
	ShowWindow(hwnd, nCmdShow);
	// �E�B���h�E�v���V�[�W����WM_PAINT���b�Z�[�W�𑗂�
	UpdateWindow(hwnd);
	return true;
}

// ���݂̃A�v���P�[�V�����̕ʂ̃C���X�^���X���Ȃ������`�F�b�N
// �߂�l�F�ʂ̃C���X�^���X�����������ꍇ�Atrue
//		�@ ���g���B��̃C���X�^���X�ł���ꍇ�Afalse
bool AnotherInstance()
{
	HANDLE ourMutex;

	// �ŗL�̕�������g���ă~���[�e�b�N�X�̍쐬�����݂�
	ourMutex = CreateMutex(NULL, true,
		"MUTEX");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;	// �ʂ̃C���X�^���X�����������ꍇ
	return false;		// ���g���B��̃C���X�^���X�ł���ꍇ
}