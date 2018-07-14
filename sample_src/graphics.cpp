//==========================================================
/// @file
/// @brief    graphics.h�̎���
/// @author   �������V
///
/// @attention  �X�v���C�g�Ȃǂ�`�悷�邽�߂̏�����������A�`�揈�����s���N���X�ł��B


#include "graphics.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Graphics::Graphics()
{
	mDirect3d = NULL;
	mDevice3d = NULL;
	mSprite = NULL;
	mFullscreen = false;
	mWidth = GAME_WIDTH;    // ���ƍ�����initialize()�Œu����������
	mHeight = GAME_HEIGHT;
	mBackColor = graphicsNS::BACK_COLOR;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Graphics::~Graphics()
{
	releaseAll();
}

//=============================================================================
// ���ׂĂ����
//=============================================================================
void Graphics::releaseAll()
{
	safeRelease(mSprite);
	safeRelease(mDevice3d);
	safeRelease(mDirect3d);
}

//=============================================================================
// DirextX�O���t�B�b�N�X��������
// �G���[����GameError���X���[
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	mHwnd = hw;
	mWidth = w;
	mHeight = h;
	mFullscreen = full;

	// Direct3D�f�o�C�X���쐬
	mDirect3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (mDirect3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

	initD3Dpp();        // D3D�v���[���e�[�V�����p�����[�^��������
	if (mFullscreen)    // �S��ʕ\�����[�h�̏ꍇ
	{
		if (isAdapterCompatible())   // �A�_�v�^�[���Ή����Ă��邩
			// �Ή����Ă��郊�t���b�V�����[�g��ݒ�
			mD3dpp.FullScreen_RefreshRateInHz = mPMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR,
				"The graphics device does not support the specified resolution and/or format."));
	}

	// �O���t�B�b�N�X�J�[�h���n�[�h�E�F�A�e�N�X�`�������O�ƃ��C�e�B���O�A
	// ���_�V�F�[�_�[���T�|�[�g���Ă��邩�ǂ����𔻒�
	D3DCAPS9 caps;
	DWORD behavior;
	mResult = mDirect3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	// �f�o�C�X��HW�e�N�X�`�������O�ƃ��C�e�B���O���T�|�[�g���Ă��Ȃ��ꍇ�A
	// �܂��́A1.1���_�V�F�[�_�[���n�[�h�E�F�A�ŃT�|�[�g���Ă��Ȃ��ꍇ�A
	// �\�t�g�E�F�A���_�����ɐ؂�ւ��܂��B
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		// �\�t�g�E�F�A�݂̂̏������g�p
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		// �n�[�h�E�F�A�݂̂̏����̎g�p
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// Direct3D�f�o�C�X���쐬
	mResult = mDirect3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		mHwnd,
		behavior,
		&mD3dpp,
		&mDevice3d);

	if (FAILED(mResult))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));

	mResult = D3DXCreateSprite(mDevice3d, &mSprite);
	if (FAILED(mResult))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));

	// �v���~�e�B�u�̃A���t�@�u�����h�p�̍\��
	mDevice3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	mDevice3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mDevice3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// D3D�v���[���e�[�V�����p�����[�^��������
//=============================================================================
void Graphics::initD3Dpp()
{
	try {
		ZeroMemory(&mD3dpp, sizeof(mD3dpp));            // �\���̂�0��ݒ�
		// �K�v�ȃp�����[�^��ݒ�
		mD3dpp.BackBufferWidth = mWidth;
		mD3dpp.BackBufferHeight = mHeight;
		if (mFullscreen)                                // �S��ʕ\���̏ꍇ
			mD3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	// 24�r�b�g�J���[
		else
			mD3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // �f�X�N�g�b�v�ݒ���g�p
		mD3dpp.BackBufferCount = 1;
		mD3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		mD3dpp.hDeviceWindow = mHwnd;
		mD3dpp.Windowed = (!mFullscreen);
		mD3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		mD3dpp.EnableAutoDepthStencil = true;
		mD3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;   // Depth 24, Stencil 8
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing D3D presentation parameters"));
	}
}

//=============================================================================
// �e�N�X�`�����f�t�H���g��D3D�������ɓǂݍ��ށi�ʏ�̃e�N�X�`���Ŏg�p�j
// �G���W���̎g�p���ɓ����I�ɂ̂ݎg�p���܂��B
// �Q�[���e�N�X�`���̓ǂݍ��݂ɂ́ATextureManager�N���X���g�p���܂��B
// ���s�O�Ffilename�́A�e�N�X�`���t�@�C���̖��O
//		�@ transcolor�́A�����Ƃ��Ĉ����F
// ���s��Fwidth��height = �e�N�X�`���̐��@
//         texture�́A�e�N�X�`�����w��
// HRESULT��߂�
//=============================================================================
HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor,
	UINT &width, UINT &height, LP_TEXTURE &texture)
{
	// �t�@�C������ǂݎ�邽�߂̍\����
	D3DXIMAGE_INFO info;
	mResult = E_FAIL;

	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// ���ƍ������t�@�C������擾
		mResult = D3DXGetImageInfoFromFile(filename, &info);
		if (mResult != D3D_OK)
			return mResult;
		width = info.Width;
		height = info.Height;

		// �t�@�C����ǂݍ���ŁA�V�����e�N�X�`�����쐬
		mResult = D3DXCreateTextureFromFileEx(
			mDevice3d,          // 3D�f�o�C�X
			filename,           // �摜�t�@�C���̖��O
			info.Width,         // �e�N�X�`���̕�
			info.Height,        // �e�N�X�`���̍���
			1,                  // �~�b�v�}�b�v�̃��x���i�`�F�[���Ȃ��̏ꍇ��1�j
			0,                  // �g�p���@�iusage�j
			D3DFMT_UNKNOWN,     // �T�[�t�F�C�X�t�H�[�}�b�g�i�f�t�H���g�j
			D3DPOOL_DEFAULT,    // �e�N�X�`���p�̃������̎��
			D3DX_DEFAULT,       // �摜�t�B���^
			D3DX_DEFAULT,       // �~�b�v�t�B���^
			transcolor,         // �����p�̐F�L�[
			&info,              // �s�b�g�}�b�v�t�@�C�����i�ǂݍ��񂾃t�@�C������j
			NULL,               // �J���[�p���b�g
			&texture);          // �ړI�̃e�N�X�`��

	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return mResult;
}

//=============================================================================
// �e�N�X�`�����V�X�e���������ɓǂݍ��ށi�V�X�e���������̓��b�N�\�j
// �s�N�Z���f�[�^�ւ̒��ڃA�N�Z�X���\�ɂ��܂��B
// TextureManager�N���X���g���āA�\������e�N�X�`����ǂݍ��݂܂��B
// ���s�O�Ffilename�́A�e�N�X�`���t�@�C���̖��O
//		   transcolor�́A�����Ƃ��Ĉ����F
// ���s��Fwidth��height = �e�N�X�`���̐��@
//         texture�́A�e�N�X�`�����w��
// HRESULT��߂��ATextureData�\���̂Ƀf�[�^���i�[����
//=============================================================================
HRESULT Graphics::loadTextureSystemMem(const char *filename, COLOR_ARGB transcolor,
	UINT &width, UINT &height, LP_TEXTURE &texture)
{
	// �r�b�g�}�b�v�t�@�C������ǂݎ�邽�߂̍\����
	D3DXIMAGE_INFO info;
	mResult = E_FAIL;        // �W����Windows�߂�l

	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// ���ƍ������r�b�g�}�b�v�t�@�C������擾
		mResult = D3DXGetImageInfoFromFile(filename, &info);
		if (mResult != D3D_OK)
			return mResult;
		width = info.Width;
		height = info.Height;

		// �r�b�g�}�b�v�摜�t�@�C����ǂݍ���ŁA�V�����e�N�X�`�����쐬
		mResult = D3DXCreateTextureFromFileEx(
			mDevice3d,          // 3D�f�o�C�X
			filename,           // �r�b�g�}�b�v�t�@�C���̖��O
			info.Width,         // �r�b�g�}�b�v�摜�̕�
			info.Height,        // �r�b�g�}�b�v�摜�̍���
			1,                  // �~�b�v�}�b�v�̃��x���i�`�F�[���Ȃ��̏ꍇ��1�j
			0,                  // �g�p���@�iusage�j
			D3DFMT_UNKNOWN,     // �T�[�t�F�C�X�t�H�[�}�b�g�i�f�t�H���g�j
			D3DPOOL_SYSTEMMEM,  // �V�X�e���������̓��b�N�\
			D3DX_DEFAULT,       // �摜�t�B���^
			D3DX_DEFAULT,       // �~�b�v�t�B���^
			transcolor,         // �����p�̐F�L�[
			&info,              // �r�b�g�}�b�v�t�@�C�����i�ǂݍ��񂾃t�@�C������j
			NULL,               // �J���[�p���b�g
			&texture);          // �ړI�̃e�N�X�`��

	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return mResult;
}

//=============================================================================
// ���_�o�b�t�@���쐬
// ���s�O�Fverts[]�ɒ��_�f�[�^���i�[����Ă���
//		   size = verts[]�̃T�C�Y
// ���s��F���������ꍇ�A&vertexBuffer���o�b�t�@���w��
//=============================================================================
HRESULT Graphics::createVertexBuffer(VertexC verts[], UINT size, LP_VERTEXBUFFER &vertexBuffer)
{
	// �W����Windows�߂�l
	HRESULT result = E_FAIL;

	// ���_�o�b�t�@���쐬
	result = mDevice3d->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, D3DFVF_VERTEX,
		D3DPOOL_DEFAULT, &vertexBuffer, NULL);
	if (FAILED(result))
		return result;

	void *ptr;
	// �f�[�^���]������Ă���O�ɁA�o�b�t�@�����b�N����K�v������
	result = vertexBuffer->Lock(0, size, (void**)&ptr, 0);
	if (FAILED(result))
		return result;
	memcpy(ptr, verts, size);   // ���_�f�[�^���o�b�t�@�ɃR�s�[
	vertexBuffer->Unlock();     // �o�b�t�@�̃��b�N������

	return result;
}

//=============================================================================
// �O�p�`�t�@�����g���āA�A���t�@���ߐ������l�p�`��\��
// ���s�O�FcreateVertexBuffer���g����vertexBuffer���쐬���A
//		   �l�p�`�����v���̏����Œ�`����4�̒��_���i�[���Ă���
//		   g3ddev->BeginScene���Ăяo��
// ���s��F�l�p�`���`�悳���
//=============================================================================
bool Graphics::drawQuad(LP_VERTEXBUFFER vertexBuffer)
{
	HRESULT result = E_FAIL;    // �W����Windows�߂�l

	if (vertexBuffer == NULL)
		return false;

	// �A���t�@�u�����h��L����
	mDevice3d->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	mDevice3d->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexC));
	mDevice3d->SetFVF(D3DFVF_VERTEX);
	result = mDevice3d->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// �A���t�@�u�����h�𖳌���
	mDevice3d->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	if (FAILED(result))
		return false;

	return true;
}

//=============================================================================
// �o�b�N�o�b�t�@��\��
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
	// �o�b�N�o�b�t�@����ʂɕ\��
	mResult = mDevice3d->Present(NULL, NULL, NULL, NULL);
	return mResult;
}

//=============================================================================
// �A�_�v�^�[���`�F�b�N���āAd3dpp�Ŏw�肳�ꂽBackBuffer�̍����A���A
// ���t���b�V�����[�g�ɑΉ����Ă��邩�ǂ������m�F���܂��B
// �Ή�������̂����������ꍇ�́ApMode�\���̂ɁA
// �Ή����Ă��郂�[�h�̃t�H�[�}�b�g��ݒ肵�܂��B
// ���s�O�Fd3dpp������������Ă���
// ���s��F�Ή����Ă��郂�[�h��������ApMode�\���̂Ƀf�[�^��
// �ݒ肳�ꂽ�ꍇ�Atrue��߂��܂��B
// �Ή����Ă��郂�[�h��������Ȃ������ꍇ�Afalse��߂��܂��B
//=============================================================================
bool Graphics::isAdapterCompatible()
{
	UINT modes = mDirect3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,
		mD3dpp.BackBufferFormat);
	for (UINT i = 0; i<modes; i++)
	{
		mResult = mDirect3d->EnumAdapterModes(D3DADAPTER_DEFAULT,
			mD3dpp.BackBufferFormat,
			i, &mPMode);
		if (mPMode.Height == mD3dpp.BackBufferHeight &&
			mPMode.Width == mD3dpp.BackBufferWidth &&
			mPMode.RefreshRate >= mD3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}

//=============================================================================
// SpriteData�\���̂ɋL�q���ꂽ�X�v���C�g��`��
// color�i�I�v�V�����j�́A�t�B���^�̂悤�ɓK�p�����
// �f�t�H���g�͔��i�ω��Ȃ��j
// ���s�O�Fsprite->Begin()���Ăяo��
// ���s��Fsprite->End()���Ăяo��
//		   spriteDate.rect�́A�`�悷��spriteDate.texture�̕������`
//		   spriteData.rect.right�́A�E�[+1�ɂ���K�v������
//		   spriteDate.rect.bottom�́A���[+1�ɂ���K�v������
//=============================================================================
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
	if (spriteData.texture == NULL)      // �e�N�X�`�����Ȃ��ꍇ
		return;

	// �X�v���C�g�̒��S�����
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale),
		(float)(spriteData.height / 2 * spriteData.scale));
	// �X�v���C�g�̉�ʈʒu
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);
	// X������Y�����̊g��k��
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);
	if (spriteData.flipHorizontal)  // ���������ɔ��]����ꍇ
	{
		scaling.x *= -1;            // X�X�P�[���𕉂ɂ��Ĕ��]
		// ���]���ꂽ�摜�̒��S���擾
		spriteCenter.x -= (float)(spriteData.width*spriteData.scale);
		// ���[�𒆐S�ɔ��]���N����̂ŁA���]���ꂽ�摜��
		// ���Ɠ����ʒu�ɔz�u���邽�߂ɁA�E�ֈړ����܂��B
		translate.x += (float)(spriteData.width*spriteData.scale);
	}
	if (spriteData.flipVertical)    // ���������ɔ��]����ꍇ
	{
		scaling.y *= -1;            // Y�X�P�[���𕉂ɂ��Ĕ��]
		// ���]���ꂽ�摜�̒��S���擾
		spriteCenter.y -= (float)(spriteData.height*spriteData.scale);
		// ��[�𒆐S�ɔ��]���N����̂ŁA���]���ꂽ�摜��
		// ���Ɠ����ʒu�ɔz�u���邽�߂ɁA���ֈړ����܂��B
		translate.y += (float)(spriteData.height*spriteData.scale);
	}
	// �X�v���C�g�̉�]�A�g��k���A�z�u���s�����߂̍s����쐬
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,                // �s��
		NULL,                   // �g��k�����s���Ƃ��A���_�͍���̂܂�
		0.0f,                   // �g��k�����̉�]�Ȃ�
		&scaling,               // �g��k���̔{��
		&spriteCenter,          // ��]�̒��S
		(float)(spriteData.angle),  // ��]�̊p�x
		&translate);            // X�AY�ʒu

	// �s����X�v���C�g�ɓK�p
	mSprite->SetTransform(&matrix);

	// �X�v���C�g��`��
	mSprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

//=============================================================================
// �E�B���h�E���[�h��t���X�N���[�����[�h�ɐݒ�
// ���s�O�F���ׂĂ�D3DPOOL_DEFAULT�T�[�t�F�C�X���������
// ���s��F���ׂẴT�[�t�F�C�X���č쐬
//=============================================================================
void Graphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	try {
		switch (mode)
		{
		case graphicsNS::FULLSCREEN:
			if (mFullscreen)			// ���łɃt���X�N���[�����[�h�������ꍇ�͉������Ȃ�
				return;
			mFullscreen = true; break;
		case graphicsNS::WINDOW:
			if (mFullscreen == false)	// ���łɃE�B���h�E���[�h�������ꍇ�͉������Ȃ�
				return;
			mFullscreen = false; break;
		default:			// �f�t�H���g�ł̓t���X�N���[�����[�h�ƃE�B���h�E���[�h��؂�ւ���
			mFullscreen = !mFullscreen;
		}
		reset();
		if (mFullscreen)	// �t���X�N���[�����[�h
		{
			SetWindowLong(mHwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else				// �E�B���h�E���[�h
		{
			SetWindowLong(mHwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(mHwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			// �N���C�A���g�̈悪GAME_WIDTH�~GAME_HEIGHT�ƂȂ�悤�ɃE�B���h�E�T�C�Y�𒲐�
			RECT clientRect;
			GetClientRect(mHwnd, &clientRect);   // �E�B���h�E�̃N���C�A���g�̈�̃T�C�Y���m��
			MoveWindow(mHwnd,
				0,                                           // ��
				0,                                           // ��
				GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // �E
				GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // ��
				TRUE);                                       // �E�B���h�E�̍ĕ`��
		}

	}
	catch (...)
	{
		// �G���[�����������ꍇ�A�E�B���h�E���[�h�ɐ؂�ւ��悤�Ƃ���
		SetWindowLong(mHwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(mHwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		// �N���C�A���g�̈悪GAME_WIDTH�~GAME_HEIGHT�ƂȂ�悤�ɃE�B���h�E�T�C�Y�𒲐�
		RECT clientRect;
		GetClientRect(mHwnd, &clientRect);   // �E�B���h�E�̃N���C�A���g�̈�̃T�C�Y���m��
		MoveWindow(mHwnd,
			0,                                           // ��
			0,                                           // ��
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // �E
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // ��
			TRUE);                                       // �E�B���h�E�̍ĕ`��
	}
}

//=============================================================================
// �f�o�C�X���������Ă��Ȃ������`�F�b�N
//=============================================================================
HRESULT Graphics::getDeviceState()
{
	mResult = E_FAIL;    // �������f�t�H���g�Ƃ��A�������ɒu������
	if (mDevice3d == NULL)
		return  mResult;
	mResult = mDevice3d->TestCooperativeLevel();
	return mResult;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g
//=============================================================================
HRESULT Graphics::reset()
{
	initD3Dpp();						 // D3D�v���[���e�[�V�����p�����[�^��������
	mSprite->OnLostDevice();             // �X�v���C�g�����
	// �O���t�B�b�N�X�f�o�C�X�̃��Z�b�g�����݂�
	mResult = mDevice3d->Reset(&mD3dpp);

	// �v���~�e�B�u�̃A���t�@�u�����h�p�̍\��
	mDevice3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	mDevice3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mDevice3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	mSprite->OnResetDevice();            // �X�v���C�g���Ċm��
	return mResult;
}
