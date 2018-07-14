//==========================================================
/// @file
/// @brief    game.h�̎���
/// @author   �������V
///
/// @attention  �Q�[���G���W���̊�ՂƂȂ�N���X�ł��B
///				�Q�[���̃��C���ƂȂ�N���X�͂��̃N���X���p������K�v������܂��B

//==========================================================

#include "game.h"

//==========================================================


//=============================================================================
// �R���X�g���N�^
//=============================================================================
Game::Game()
{
	mInput = new Input();        // �L�[�{�[�h���͂𑦎��ɏ�����
	// ���̑��̏������́A���
	// input->initialize()���Ăяo���ď���
	mPaused = false;             // �Q�[���͈ꎞ��~���łȂ�
	mGraphics = NULL;
	mAudio = NULL;
	mConsole = NULL;
	mMessageDialog = NULL;
	mInputDialog = NULL;
	mFps = 100;
	mFpsOn = false;              // �f�t�H���g�ł̓t���[�����[�g��\�����Ȃ�
	mInitialized = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Game::~Game()
{
	deleteAll();                // �\�񂳂�Ă��������������ׂĉ��
	ShowCursor(true);           // �J�[�\����\��
}

//=============================================================================
// Window���b�Z�[�W�n���h��
//=============================================================================
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (mInitialized)     // ����������Ă��Ȃ��ꍇ�̓��b�Z�[�W���������Ȃ�
	{
		switch (msg)
		{
		case WM_DESTROY:
			// Windows�ɂ��̃v���O�������I������悤�ɓ`����
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:    // �L�[�������ꂽ
			mInput->keyDown(wParam);
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP:        // �L�[�������ꂽ
			mInput->keyUp(wParam);
			return 0;
		case WM_CHAR:                           // ���������͂��ꂽ
			mInput->keyIn(wParam);
			return 0;
		case WM_MOUSEMOVE:                      // �}�E�X���ړ����ꂽ
			mInput->mouseIn(lParam);
			return 0;
		case WM_INPUT:                          // �}�E�X����̃��[�f�[�^����
			mInput->mouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN:                    // ���}�E�X�{�^���������ꂽ
			mInput->setMouseLButton(true);
			mInput->mouseIn(lParam);            // �}�E�X�̈ʒu
			return 0;
		case WM_LBUTTONUP:                      // ���}�E�X�{�^���������ꂽ
			mInput->setMouseLButton(false);
			mInput->mouseIn(lParam);            // �}�E�X�̈ʒu
			return 0;
		case WM_MBUTTONDOWN:                    // �����}�E�X�{�^���������ꂽ
			mInput->setMouseMButton(true);
			mInput->mouseIn(lParam);            // �}�E�X�̈ʒu
			return 0;
		case WM_MBUTTONUP:                      // �����}�E�X�{�^���������ꂽ
			mInput->setMouseMButton(false);
			mInput->mouseIn(lParam);            // �}�E�X�̈ʒu
			return 0;
		case WM_RBUTTONDOWN:                    // �E�}�E�X�{�^���������ꂽ
			mInput->setMouseRButton(true);
			mInput->mouseIn(lParam);            // �}�E�X�̈ʒu
			return 0;
		case WM_RBUTTONUP:                      // �E�}�E�X�{�^���������ꂽ
			mInput->setMouseRButton(false);
			mInput->mouseIn(lParam);            // �}�E�X�̈ʒu
			return 0;
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:	// �}�E�X��X�{�^���������ꂽ/�����ꂽ
			mInput->setMouseXButton(wParam);
			mInput->mouseIn(lParam);            // �}�E�X�̈ʒu
			return 0;
		case WM_MOUSEWHEEL:                     // �}�E�X�z�C�[����������
			mInput->mouseWheelIn(wParam);
			return 0;
		case WM_DEVICECHANGE:                   // �R���g���[���[���`�F�b�N
			mInput->checkControllers();
			return 0;
		}
	}
	// Windows�ɏ�����C����
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//=============================================================================
// �Q�[����������
// �G���[����GameError���X���[
//=============================================================================
void Game::initialize(HWND hw)
{
	mHwnd = hw;									// �E�B���h�E�n���h����ۑ�

	// �O���t�B�b�N�X��������
	mGraphics = new Graphics();
	// GameError���X���[
	mGraphics->initialize(mHwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// ���͂��������A�}�E�X���L���v�`�����Ȃ�
	mInput->initialize(mHwnd, false);           // GameError���X���[

	// �R���\�[����������
	mConsole = new Console();
	mConsole->initialize(mGraphics, mInput);    // �R���\�[��������
	mConsole->print("---Console---");

	// messageDialog��������
	mMessageDialog = new MessageDialog();
	mMessageDialog->initialize(mGraphics, mInput, mHwnd);

	// initialize inputDialog
	mInputDialog = new InputDialog();
	mInputDialog->initialize(mGraphics, mInput, mHwnd);

	// DirectX�t�H���g��������
	if (mDxFont.initialize(mGraphics, gameNS::POINT_SIZE, false, false, gameNS::FONT) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize DirectX font."));

	mDxFont.setFontColor(gameNS::FONT_COLOR);

	// �T�E���h�V�X�e����������
	mAudio = new Audio();
	// �T�E���h�t�@�C������`����Ă���ꍇ
	if (*WAVE_BANK != '\0' && *SOUND_BANK != '\0')
	{
		if (FAILED(mHr = mAudio->initialize()))
		{
			if (mHr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system because media file not found."));
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize sound system."));
		}
	}

	// ������\�^�C�}�[�̏��������݂�
	if (QueryPerformanceFrequency(&mTimerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

	QueryPerformanceCounter(&mTimeStart);        // �J�n���Ԃ��擾

	mInitialized = true;
}

//=============================================================================
// �Q�[���A�C�e���������_�[
//=============================================================================
void Game::renderGame()
{
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];

	// �����_�����O���J�n
	if (SUCCEEDED(mGraphics->beginScene()))
	{
		// render�́A�p�������N���X���ŋL�q����K�v�̂��鏃�����z�֐��ł��B
		render();	// �h���N���X��render���Ăяo��

		mGraphics->spriteBegin();    // �X�v���C�g�̕`����J�n
		if (mFpsOn)					 // �t���[�����[�g�̕\�����v������Ă���ꍇ
		{
			// fps�𕶎���ɕϊ�
			_snprintf_s(buffer, BUF_SIZE, "fps %d ", (int)mFps);
			mDxFont.print(buffer, GAME_WIDTH - 100, GAME_HEIGHT - 28);
		}
		mGraphics->spriteEnd();      // �X�v���C�g�̕`����I��

		mConsole->draw();			// �R���\�[���́A�Q�[���̑O�ʂɕ\�������悤�ɂ����ŕ`��
		mMessageDialog->draw();		// ���b�Z�[�W�_�C�A���O��O�ʂɕ`��
		mInputDialog->draw();		// ���͗p�_�C�A���O������ɑO�ʂɕ`��

		// �����_�����O���I��
		mGraphics->endScene();
	}
	handleLostGraphicsDevice();

	// �o�b�N�o�b�t�@����ʂɕ\��
	mGraphics->showBackbuffer();
}

//=============================================================================
// ���������O���t�B�b�N�X�f�o�C�X������
//=============================================================================
void Game::handleLostGraphicsDevice()
{
	// �f�o�C�X�̏������e�X�g���A����ɉ����ď��������s
	mHr = mGraphics->getDeviceState();
	if (FAILED(mHr))					// �O���t�B�b�N�X�f�o�C�X���L���ȏ�ԂłȂ��ꍇ
	{
		// �f�o�C�X���������Ă���A���Z�b�g�ł����ԂɂȂ��ꍇ
		if (mHr == D3DERR_DEVICELOST)
		{
			Sleep(100);					// CPU���Ԃ𖾂��n���i100�~���b�j
			return;
		}
		// �f�o�C�X���������Ă��邪�A���Z�b�g�ł����Ԃɂ���ꍇ
		else if (mHr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			mHr = mGraphics->reset();	// �O���t�B�b�N�X�f�o�C�X�̃��Z�b�g�����݂�
			if (FAILED(mHr))			// ���Z�b�g�����s�����ꍇ
				return;
			resetAll();
		}
		else
			return;						// ���̃f�o�C�X�G���[
	}
}

//=============================================================================
// �f�B�X�v���C���[�h���Z�b�g�i�t���X�N���[���A�E�B���h�E�������̓g�O���j
//=============================================================================
void Game::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	// �\�񂳂�Ă����r�f�I�����������ׂĉ��
	releaseAll(); 
	mGraphics->changeDisplayMode(mode);
	// ���ׂẴT�[�t�F�C�X���č쐬���A���ׂẴG���e�B�e�B�����Z�b�g
	resetAll();
}

//=============================================================================
// WimMain���̃��C���̃��b�Z�[�W���[�v�ŌJ��Ԃ��Ăяo�����
//=============================================================================
void Game::run(HWND hwnd)
{
	if (mGraphics == NULL)            // �O���t�B�b�N�X������������Ă��Ȃ��ꍇ
		return;

	// �Ō�̃t���[������̌o�ߎ��Ԃ��v�Z�AframeTime�ɕۑ�
	QueryPerformanceCounter(&mTimeEnd);
	mFrameTime = (float)(mTimeEnd.QuadPart - mTimeStart.QuadPart) / (float)mTimerFreq.QuadPart;

	// �ȓd�̓R�[�h�iwinmm.lib���K�v�j
	// ��]����t���[�����[�g�ɑ΂��Čo�ߎ��Ԃ��Z���ꍇ
	if (mFrameTime < MIN_FRAME_TIME)
	{
		mSleepTime = (DWORD)((MIN_FRAME_TIME - mFrameTime) * 1000);
		timeBeginPeriod(1);         // 1�~���b�̕���\��Windows�^�C�}�[�ɗv��
		Sleep(mSleepTime);          // sleepTime�̊ԁACPU�����
		timeEndPeriod(1);           // 1�~���b�̃^�C�}�[����\���I��
		return;
	}

	if (mFrameTime > 0.0)
		mFps = (mFps*0.99f) + (0.01f / mFrameTime);	// ����fps
	if (mFrameTime > MAX_FRAME_TIME)				// �t���[�����[�g�����ɒx���ꍇ
		mFrameTime = MAX_FRAME_TIME;				// �ő�frameTime�𐧌�
	mTimeStart = mTimeEnd;

	// update()�Aai()�Acollisions()�͏������z�֐��ł��B
	// �����̊֐��́AGame���p�����Ă���N���X���ŋL�q����K�v������܂��B
	if (!mPaused)								// �ꎞ��~���łȂ��ꍇ
	{
		update();								// ���ׂẴQ�[���A�C�e�����X�V
		ai();									// �l�H�m�\
		collisions();							// �Փ˂�����
		mInput->vibrateControllers(mFrameTime);	// �R���g���[���[�̐U��������
	}
	renderGame();								// ���ׂẴQ�[���A�C�e����`��

	// �R���\�[���L�[���`�F�b�N
	if (mInput->getCharIn() == CONSOLE_KEY)
	{
		mInput->clearCharIn();       // �Ō�ɓ��͂��ꂽ�������N���A
		mConsole->showHide();
		// �R���\�[�����\������Ă���ԁA�Q�[�����ꎞ��~
		mPaused = mConsole->getVisible();
	}
	consoleCommand();               // ���[�U�[�����͂����R���\�[���R�}���h������

	mInput->readControllers();		// �R���g���[�^�[�̏�Ԃ�ǂݎ��
	mMessageDialog->update();		// �{�^���N���b�N���`�F�b�N
	mInputDialog->update();

	mAudio->run();                  // �T�E���h�G���W���̎����I�^�X�N�����s

	// Alt+Enter�Ńt���X�N���[�����[�h�ƃE�B���h�E���[�h��؂�ւ�
//	if (mInput->isKeyDown(ALT_KEY) && mInput->wasKeyPressed(ENTER_KEY))
//		setDisplayMode(graphicsNS::TOGGLE); // �t���X�N���[�����[�h�ƃE�B���h�E���[�h��؂�ւ�

	// Esc�L�[�ŃE�B���h�E���[�h�ɐݒ�
//	if (mInput->isKeyDown(ESC_KEY))
//		setDisplayMode(graphicsNS::WINDOW); // �E�B���h�E���[�h�ɐݒ�

	// �|�[�Y�L�[�������ꂽ�ꍇ�A�ꎞ��~
	if (mInput->wasKeyPressed(VK_PAUSE))
		mPaused = !mPaused;

	// ���͂��N���A
	// ���ׂẴL�[�`�F�b�N���s��ꂽ�ケ����Ăяo��
	mInput->clear(inputNS::KEYS_PRESSED);
}

//=============================================================================
// �R���\�[���R�}���h������
// �V�����R���\�[���R�}���h��ǉ�����ꍇ�́A
// ���̊֐���h���N���X�ŃI�[�o�[���C�h����
//=============================================================================
void Game::consoleCommand()
{
	mCommand = mConsole->getCommand();   // �R���\�[������̃R�}���h���擾
	if (mCommand == "")                  // �R�}���h���Ȃ��ꍇ
		return;

	if (mCommand == "help")              // �uhelp�v�R�}���h�̏ꍇ
	{
		mConsole->print("Console Commands:");
		mConsole->print("fps - toggle display of frames per second");
		return;
	}

	if (mCommand == "fps")
	{
		mFpsOn = !mFpsOn;                // �t���[�����[�g�̕\����؂�ւ���
		if (mFpsOn)
			mConsole->print("fps On");
		else
			mConsole->print("fps Off");
	}
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
//=============================================================================
void Game::releaseAll()
{
	safeOnLostDevice(mInputDialog);
	safeOnLostDevice(mMessageDialog);
	safeOnLostDevice(mConsole);
	mDxFont.onLostDevice();
	return;
}

//=============================================================================
// ���ׂẴT�[�t�F�C�X���č쐬���A���ׂẴG���e�B�e�B�����Z�b�g
//=============================================================================
void Game::resetAll()
{
	mDxFont.onResetDevice();
	safeOnResetDevice(mConsole);
	safeOnResetDevice(mMessageDialog);
	safeOnResetDevice(mInputDialog);
	return;
}

//=============================================================================
// �\�񂳂�Ă��������������ׂč폜
//=============================================================================
void Game::deleteAll()
{
	// ���ׂẴO���t�B�b�N�X�A�C�e���ɂ���onLostDevice()���Ăяo��
	releaseAll();
	safeDelete(mAudio);
	safeDelete(mGraphics);
	safeDelete(mInput);
	safeDelete(mConsole);
	safeDelete(mMessageDialog);
	safeDelete(mInputDialog);
	mInitialized = false;
}