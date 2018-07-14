//==========================================================
/// @file
/// @brief    rect.h�̎���
/// @author   �������V
///
/// @attention  �l�p�`��`�悷��N���X�ł��B
///				�V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p�ł��܂��B

//==========================================================
#include "rect.h"
//==========================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Rect::Rect()
{
	mInitialized = false;                // ����ɏ��������ꂽ�ꍇ�Atrue��ݒ�
	mGraphics = NULL;
	mBackColor = rectNS::BACK_COLOR;

	mX = rectNS::X;						// �ŏ��̎l�p�`�̈ʒu
	mY = rectNS::Y;

	mWidth = rectNS::WIDTH;
	mHeight = rectNS::HEIGHT;

	mVertexBuffer = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Rect::~Rect()
{
	onLostDevice();
}

//=============================================================================
// Rect��������
//=============================================================================
bool Rect::initialize(Graphics *g)
{
	try {
		mGraphics = g;                    // �O���t�B�b�N�V�X�e��

		// ����
		mVtx[0].x = mX;
		mVtx[0].y = mY;
		mVtx[0].z = 0.0f;
		mVtx[0].rhw = 1.0f;
		mVtx[0].color = mBackColor;

		// �E��
		mVtx[1].x = mX + mWidth;
		mVtx[1].y = mY;
		mVtx[1].z = 0.0f;
		mVtx[1].rhw = 1.0f;
		mVtx[1].color = mBackColor;

		// �E��
		mVtx[2].x = mX + mWidth;
		mVtx[2].y = mY + mHeight;
		mVtx[2].z = 0.0f;
		mVtx[2].rhw = 1.0f;
		mVtx[2].color = mBackColor;

		// ����
		mVtx[3].x = mX;
		mVtx[3].y = mY + mHeight;
		mVtx[3].z = 0.0f;
		mVtx[3].rhw = 1.0f;
		mVtx[3].color = mBackColor;

		mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);

	}
	catch (...) {
		return false;
	}

	mInitialized = true;
	return true;
}


//=============================================================================
// reset
//=============================================================================
void Rect::reset()
{
	// ����
	mVtx[0].x = mX;
	mVtx[0].y = mY;
	mVtx[0].z = 0.0f;
	mVtx[0].rhw = 1.0f;
	mVtx[0].color = mBackColor;

	// �E��
	mVtx[1].x = mX + mWidth;
	mVtx[1].y = mY;
	mVtx[1].z = 0.0f;
	mVtx[1].rhw = 1.0f;
	mVtx[1].color = mBackColor;

	// �E��
	mVtx[2].x = mX + mWidth;
	mVtx[2].y = mY + mHeight;
	mVtx[2].z = 0.0f;
	mVtx[2].rhw = 1.0f;
	mVtx[2].color = mBackColor;

	// ����
	mVtx[3].x = mX;
	mVtx[3].y = mY + mHeight;
	mVtx[3].z = 0.0f;
	mVtx[3].rhw = 1.0f;
	mVtx[3].color = mBackColor;

	mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);

}

//=============================================================================
// �l�p�`��`��
// ���s�O�FBeginScene��EndScene�̓����ɂ���
//=============================================================================
const void Rect::draw()
{
	mGraphics->drawQuad(mVertexBuffer);       // �w�i��`��

	mGraphics->spriteBegin();                // �X�v���C�g�̕`����J�n

	mGraphics->spriteEnd();                      // �X�v���C�g�̕`����I��
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
//=============================================================================
void Rect::onLostDevice()
{
	if (!mInitialized)
		return;
	SAFE_RELEASE(mVertexBuffer);
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
//=============================================================================
void Rect::onResetDevice()
{
	if (!mInitialized)
		return;
	mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mVertexBuffer);
}

