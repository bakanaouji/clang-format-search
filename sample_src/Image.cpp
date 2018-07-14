//==========================================================
/// @file
/// @brief    image.h�̎���
/// @author   �������V
///
/// @attention  �摜��`�悷��N���X�ł��B
///				�Q�[�����̃A�C�e���͂��ׂĉ摜������ƍl������̂ŁA
///				���̃N���X���p�����邱�Ƃ����߂܂�

//==========================================================
#include "image.h"
//==========================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
Image::Image()
{
	mInitialized = false;            // �������ɐ��������ꍇ�Atrue�ƂȂ�
	mSpriteData.width = 2;
	mSpriteData.height = 2;
	mSpriteData.x = 0.0;
	mSpriteData.y = 0.0;
	mSpriteData.scale = 1.0;
	mSpriteData.angle = 0.0;
	mSpriteData.rect.left = 0;       // �����̃t���[�������摜����P�̃t���[����I�����邽�߂Ɏg�p
	mSpriteData.rect.top = 0;
	mSpriteData.rect.right = mSpriteData.width;
	mSpriteData.rect.bottom = mSpriteData.height;
	mSpriteData.texture = NULL;      // �X�v���C�g�e�N�X�`���i�摜�j
	mSpriteData.flipHorizontal = false;
	mSpriteData.flipVertical = false;
	mCols = 1;
	mTextureManager = NULL;
	mStartFrame = 0;
	mEndFrame = 0;
	mCurrentFrame = 0;
	mFrameDelay = 1.0;					// �f�t�H���g�ł�1�b��1�t���[�����A�j���[�V�������i��
	mAnimTimer = 0.0;
	mVisible = true;					// �摜�͕\����Ԃ��f�t�H���g
	mLoop = true;						// �A�j���[�V���������[�v������̂��f�t�H���g
	mAnimComplete = false;
	mGraphics = NULL;					// �O���t�B�b�N�V�X�e���ւ̃����N
	mColorFilter = graphicsNS::WHITE;	// �J���[�t�B���^�[�Ȃ��̏ꍇ�AWHITE
	mMode = imageNS::MODE::HORIZONTAL;	// �ʏ�͐����ŃA�j���[�V�������i��
}

//==========================================================
// �p�����[�^������
//==========================================================
void Image::reset()
{
	mVisible = true;
	setCurrentFrame(mStartFrame);
	mMode = imageNS::HORIZONTAL;
	// �A�j���[�V�����̓��[�v��Ԃ���X�^�[�g
	mLoop = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Image::~Image()
{}

//=============================================================================
// Image��������
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
// Graphics�ւ̃|�C���^
// Image�̕��i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̕����g�p�j
// Image�̍����i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̍������g�p�j
// �e�N�X�`�����̗񐔁i1����n�܂Łj�i0��1�Ɠ����j
// TextureManager�ւ̃|�C���^
//=============================================================================
bool Image::initialize(Graphics *g, int width, int height, int ncols,
	TextureManager *textureM)
{
	try {
		mGraphics = g;				// graphics�I�u�W�F�N�g
		mTextureManager = textureM;	// �e�N�X�`���I�u�W�F�N�g�ւ̃|�C���^
		mSpriteData.texture = mTextureManager->getTexture();
		if (width == 0)
			width = mTextureManager->getWidth();	// �S�̂̕����g�p
		mSpriteData.width = width;
		if (height == 0)
			height = mTextureManager->getHeight();	// �S�̂̍������g�p
		mSpriteData.height = height;
		mCols = ncols;
		if (mCols == 0)
			mCols = 1;								// cols��0�̏ꍇ�́A1���g�p
													// currentFrame��`�悷��spriteData.rect���\��
		mSpriteData.rect.left = (mCurrentFrame % mCols) * mSpriteData.width;
		// �E�[+1
		mSpriteData.rect.right = mSpriteData.rect.left + mSpriteData.width;
		mSpriteData.rect.top = (mCurrentFrame / mCols) * mSpriteData.height;
		// ���[+1
		mSpriteData.rect.bottom = mSpriteData.rect.top + mSpriteData.height;
	}
	catch (...)
	{
		return false;
	}
	mInitialized = true;	// ����ɏ��������ꂽ�ꍇ
	return true;
}

//=============================================================================
// color���t�B���^�Ƃ��Ďg���ĉ摜��`��
// color�p�����[�^�̓I�v�V����
// �iimage.h�ŁA�f�t�H���g�Ƃ��Ĕ������蓖�Ă��Ă���j
// ���s�O�FspriteBegin()���Ăяo��
// ���s��FspriteEnd()���Ăяo��
//=============================================================================
void Image::draw(const COLOR_ARGB color)
{
	if (!mVisible || mGraphics == NULL)
		return;
	// onReset()���Ăяo���ꂽ�Ƃ��ɐV�����e�N�X�`�����擾
	mSpriteData.texture = mTextureManager->getTexture();
	if (color == graphicsNS::FILTER)	// �t�B���^���g���ĕ`�悷��ꍇ
										// colorFilter���g�p
		mGraphics->drawSprite(mSpriteData, mColorFilter);
	else
		// �t�B���^�Ƃ���color���g�p
		mGraphics->drawSprite(mSpriteData, color);
}

//=============================================================================
// �w�肳�ꂽSpriteData���g���Ă��̉摜��`��
// ���݂�SpriteData.rect���g���āA�e�N�X�`����I��
// ���s�O�FspriteBegin()���Ăяo��
// ���s��FspriteEnd()���Ăяo��
//=============================================================================
void Image::draw(SpriteData sd, const COLOR_ARGB color, const UINT textureN)
{
	if (!mVisible || mGraphics == NULL)
		return;
	// ���̉摜�̋�`���g���ăe�N�X�`����I��
	sd.rect = mSpriteData.rect;
	// onReset()���Ăяo���ꂽ�Ƃ��ɐV�����e�N�X�`�����擾
	sd.texture = mTextureManager->getTexture(textureN);
	if (color == graphicsNS::FILTER)			// �t�B���^���g���ĕ`�悷��ꍇ
		mGraphics->drawSprite(sd, mColorFilter);// colorFilter���g�p
	else
		mGraphics->drawSprite(sd, color);
}

//=============================================================================
// update
// �ʏ�A�t���[�����Ƃ�1��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
//=============================================================================
void Image::update(const float frameTime)
{
	switch (mMode)
	{
	case imageNS::HORIZONTAL:					// �ǂݍ��񂾉摜�t�@�C���ɑ΂��Đ��������ɃA�j���[�V�������i�ޏꍇ�A
		if (mEndFrame - mStartFrame > 0)        // �A�j���[�V����������X�v���C�g�̏ꍇ
		{
			mAnimTimer += frameTime;            // ���v�̌o�ߎ���
			if (mAnimTimer > mFrameDelay)
			{
				mAnimTimer -= mFrameDelay;
				mCurrentFrame++;
				if (mCurrentFrame < mStartFrame || mCurrentFrame > mEndFrame)
				{
					if (mLoop == true)          // ���[�v����A�j���[�V�����̏ꍇ
						mCurrentFrame = mStartFrame;
					else                        // ���[�v���Ȃ��A�j���[�V�����̏ꍇ
					{
						mCurrentFrame = mEndFrame;
						mAnimComplete = true;   // �A�j���[�V�����̊���
					}
				}
				setRect();                      // spriteData.rect��ݒ�
			}
		}
		break;
	case imageNS::VERTICAL:						// �ǂݍ��񂾉摜�t�@�C���ɑ΂��Đ��������ɃA�j���[�V�������i�ޏꍇ�A
		if (mEndFrame - mStartFrame > 0)        // �A�j���[�V����������X�v���C�g�̏ꍇ
		{
			mAnimTimer += frameTime;            // ���v�̌o�ߎ���
			if (mAnimTimer > mFrameDelay)
			{
				mAnimTimer -= mFrameDelay;
				mCurrentFrame += mCols;
				if (mCurrentFrame < mStartFrame || mCurrentFrame > mEndFrame)
				{
					if (mLoop == true)          // ���[�v����A�j���[�V�����̏ꍇ
						mCurrentFrame = mStartFrame;
					else                        // ���[�v���Ȃ��A�j���[�V�����̏ꍇ
					{
						mCurrentFrame = mEndFrame;
						mAnimComplete = true;   // �A�j���[�V�����̊���
					}
				}
				setRect();                      // spriteData.rect��ݒ�
			}
		}
		break;
	}
}

//=============================================================================
// �摜�̌��݂̃t���[����ݒ�
//=============================================================================
void Image::setCurrentFrame(int c)
{
	if (c >= 0)
	{
		mAnimTimer = 0.0f;
		mCurrentFrame = c;
		mAnimComplete = false;
		setRect();                          // spriteData.rect��ݒ�
	}
}

//=============================================================================
//  spriteData.rect�����݂̃t���[����`�悷�邽�߂ɐݒ�
//=============================================================================
inline void Image::setRect()
{
	// spriteData.rect�����݂̃t���[����`�悷�邽�߂ɍč\��
	// configure spriteData.rect to draw currentFrame
	mSpriteData.rect.left = (mCurrentFrame % mCols) * mSpriteData.width;
	// right edge + 1
	mSpriteData.rect.right = mSpriteData.rect.left + mSpriteData.width;
	mSpriteData.rect.top = (mCurrentFrame / mCols) * mSpriteData.height;
	// bottom edge + 1
	mSpriteData.rect.bottom = mSpriteData.rect.top + mSpriteData.height;
}