//==========================================================
/// @file
/// @brief    Image�N���X
/// @author   �������V
///
/// @attention  �摜��`�悷��N���X�ł��B
///				�Q�[�����̃A�C�e���͂��ׂĉ摜������ƍl������̂ŁA
///				���̃N���X���p�����邱�Ƃ����߂܂�

//==========================================================
#ifndef _IMAGE_H	//���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _IMAGE_H	// ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include "textureManager.h"
#include "constants.h"

//==========================================================

// Image�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace imageNS
{
	enum MODE { HORIZONTAL, VERTICAL };	// �ǂݍ��񂾉摜�ɑ΂��ĉ��ɃA�j���[�V�����������ꍇ��HORIZONTAL�A�c�̏ꍇ��VERTICAL
}

//==========================================================
// �摜��`�悷��N���X�ł��B
// �Q�[�����̃A�C�e���͂��ׂĉ摜������ƍl������̂ŁA
// ���̃N���X���p�����邱�Ƃ����߂܂�
class Image
{
protected:
	// Image�v���p�e�B
	Graphics *mGraphics;					// graphics�ւ̃|�C���^
	TextureManager *mTextureManager;		// texture manager�ւ̃|�C���^
	// spriteData��Graphics::drawSprite()�ɂ��摜�̕`��ɕK�v�ȃf�[�^��ێ�����
	SpriteData mSpriteData;					// "graphics.h"�Œ�`����Ă���SpriteData
	COLOR_ARGB mColorFilter;				// color filter�Ƃ��ēK�p�i�����g�p�j
	int mCols;								// �����̃t���[�������X�v���C�g���̗񐔁i1����n�܂Łj
	int mStartFrame;						// �����̃t���[�������A�j���[�V�����V�[�P���X�̍ŏ��̃t���[���ԍ�
	int mEndFrame;							// �����̃t���[�������A�j���[�V�����V�[�P���X�̍Ō�̃t���[���ԍ�
	int mCurrentFrame;						// �A�j���[�V�����̌��݂̃t���[��
	double mFrameDelay;						// �A�j���[�V�����̊e�t���[����\������Ԋu�i�b�P�ʁj
	double mAnimTimer;						// �A�j���[�V�����̎��Ԃ𑪂邽�߂Ɏg�p����ϐ�
	HRESULT mHr;							// �W���̖߂�l
	bool mLoop;								// �A�j���[�V�������J��Ԃ��ꍇ�Atrue�ɐݒ�i�f�t�H���g�j
	bool mVisible;							// �摜�����ȏꍇ�Atrue
	bool mInitialized;						// �����������������ꍇ�́Atrue
	bool mAnimComplete;						// ���[�v���Ȃ��A�j���[�V�����V�[�P���X�����������Ƃ���true�ƂȂ�
	imageNS::MODE mMode;					// �ǂݍ��񂾉摜�ɑ΂��ĉ��ɃA�j���[�V�����������ꍇ��HORIZONTAL�A�c�̏ꍇ��VERTICAL

public:
	// �R���X�g���N�^
	Image();

	// �f�X�g���N�^
	virtual ~Image();

	// SpriteData�\���̂ւ̎Q�Ƃ�߂�
	const virtual SpriteData& getSpriteInfo()
	{
		return mSpriteData;
	}
	const virtual SpriteData& getSpriteData() { return mSpriteData; }


	// visible�p�����[�^�����߂�
	virtual bool getVisible()
	{
		return mVisible;
	}

	// X�ʒu��߂�
	virtual float getX()
	{
		return mSpriteData.x;
	}

	// Y�ʒu��߂�
	virtual float getY()
	{
		return mSpriteData.y;
	}

	// �{����߂�
	virtual float getScale()
	{
		return mSpriteData.scale;
	}

	// ����߂�
	virtual int   getWidth()
	{
		return mSpriteData.width;
	}

	// ������߂�
	virtual int   getHeight()
	{
		return mSpriteData.height;
	}

	// ���S�_��X��߂�
	virtual float getCenterX()
	{
		return mSpriteData.x + mSpriteData.width / 2 * getScale();
	}

	// ���S�_��Y��߂�
	virtual float getCenterY()
	{
		return mSpriteData.y + mSpriteData.height / 2 * getScale();
	}

	// ��]�p�x�i�x�P�ʁj��߂�
	virtual float getDegrees()
	{
		return mSpriteData.angle*(180.0f / (float)PI);
	}

	// ��]�p�x�i���W�A���j��߂�
	virtual float getRadians()
	{
		return mSpriteData.angle;
	}

	// �A�j���[�V�����̃t���[���Ԃ̊Ԋu��߂�
	virtual float getFrameDelay() {
		return (float)mFrameDelay;
	}

	// �J�n�t���[���̔ԍ���߂�
	virtual int   getStartFrame()
	{
		return mStartFrame;
	}

	// �I���t���[���̔ԍ���߂�
	virtual int   getEndFrame()
	{
		return mEndFrame;
	}

	// ���݂̃t���[���̔ԍ���߂�
	virtual int   getCurrentFrame()
	{
		return mCurrentFrame;
	}

	// Image��RECT�\���̂�߂�
	virtual RECT  getSpriteDataRect()
	{
		return mSpriteData.rect;
	}

	// �A�j���[�V���������̏�Ԃ�߂�
	virtual bool  getAnimationComplete()
	{
		return mAnimComplete;
	}

	// colorFilter��߂�
	virtual COLOR_ARGB getColorFilter() { return mColorFilter; }

	// X�ʒu��ݒ�
	virtual void setX(float newX)
	{
		mSpriteData.x = newX;
	}

	// Y�ʒu��ݒ�
	virtual void setY(float newY)
	{
		mSpriteData.y = newY;
	}

	// �g��k����ݒ�
	virtual void setScale(float s)
	{
		mSpriteData.scale = s;
	}

	// ��]�p�x��ݒ�i�x�P�ʁj
	// 0�x�͏�����ł��B�p�x�͎��v���ɐi�݂܂��B
	virtual void setDegrees(float deg)
	{
		mSpriteData.angle = deg*((float)PI / 180.0f);
	}

	// ��]�p�x��ݒ�i���W�A���P�ʁj
	// 0�x�͏�����ł��B�p�x�͎��v���ɐi�݂܂��B
	virtual void setRadians(float rad)
	{
		mSpriteData.angle = rad;
	}

	// ������ݒ�
	virtual void setVisible(bool v)
	{
		mVisible = v;
	}

	// �A�j���[�V�����̃t���[���Ԃ̊Ԋu��ݒ�
	virtual void setFrameDelay(float d)
	{
		mFrameDelay = d;
	}

	// �A�j���[�V�����̊J�n�t���[���ƏI���t���[����ݒ�
	virtual void setFrames(int s, int e)
	{
		mStartFrame = s; mEndFrame = e;
	}

	// �A�j���[�V�����̌��݂̃t���[����ݒ�
	virtual void setCurrentFrame(int c);

	// CurrentFrame��`�悷��spriteData.rect��ݒ�
	virtual void setRect();

	// spriteData.rect��r�ɐݒ�
	virtual void setSpriteDataRect(RECT r)
	{
		mSpriteData.rect = r;
	}

	// �A�j���[�V�������[�v��ݒ�ilp = true�̏ꍇ�Ƀ��[�v����j
	virtual void setLoop(bool lp)
	{
		mLoop = lp;
	}

	// �A�j���[�V���������u�[���l��ݒ�
	virtual void setAnimationComplete(bool a) { mAnimComplete = a; };

	// �F�t�B���^��ݒ�iWHITE���g�p����ƁA�ω����Ȃ��j
	virtual void setColorFilter(COLOR_ARGB color)
	{
		mColorFilter = color;
	}

	// TextureManager��ݒ�
	virtual void setTextureManager(TextureManager *textureM)
	{
		mTextureManager = textureM;
	}

	// MODE��ݒ�
	virtual void setMode(imageNS::MODE m)
	{
		mMode = m;
	}


	// �摜�𐅕������ɔ��]�i���j
	virtual void flipHorizontal(bool flip)
	{
		mSpriteData.flipHorizontal = flip;
	}

	// �摜�𐂒������ɔ��]
	virtual void flipVertical(bool flip)
	{
		mSpriteData.flipVertical = flip;
	}

	// Image��������
	// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
	// Graphics�ւ̃|�C���^
	// Image�̕��i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̕����g�p�j
	// Image�̍����i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̍������g�p�j
	// �e�N�X�`�����̗񐔁i1����n�܂Łj�i0��1�Ɠ����j
	// TextureManager�ւ̃|�C���^
	virtual bool initialize(Graphics *g, int width, int height, int nclos,
		TextureManager *textureM);

	// color���t�B���^�Ƃ��Ďg���ĉ摜��`��Bcolor�̃f�t�H���g�͔��B
	// color�p�����[�^�̓I�v�V����
	// �iimage.h�ŁA�f�t�H���g�Ƃ��Ĕ������蓖�Ă��Ă���j
	// ���s�O�FspriteBegin()���Ăяo��
	// ���s��FspriteEnd()���Ăяo��
	virtual void draw(const COLOR_ARGB color = graphicsNS::WHITE);

	// �w�肳�ꂽSpriteData���g���Ă��̉摜��`��
	// ���݂�SpriteData.rect���g���āA�e�N�X�`����I��
	// ���s�O�FspriteBegin()���Ăяo��
	// ���s��FspriteEnd()���Ăяo��
	virtual void draw(SpriteData sd, const COLOR_ARGB color = graphicsNS::WHITE, const UINT textureN = 0);

	virtual void update(const float frameTime);

	// �p�����[�^���Z�b�g
	virtual void reset();
};
#endif