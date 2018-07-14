//==========================================================
/// @file
/// @brief    Text�N���X
/// @author   �������V
///
/// @attention  �X�v���C�g�e�L�X�g��`�悷��N���X�ł��B

//==========================================================
#ifndef _TEXT_H                 // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _TEXT_H                 // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

class Text;

#include <string>
#include "image.h"
#include "constants.h"

//==========================================================
// Text�N���X�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace textNS
{
	enum Alignment { LEFT, RIGHT, CENTER, CENTER_MIDDLE, CENTER_BOTTOM, LEFT_BOTTOM, RIGHT_BOTTOM };	// �A���C�������g�i���[�A�E�[�A������A�����A�������A�����A�E���j
	// �t�H���g�f�[�^�\����
	struct FontData
	{
		UINT left;	// ���ʒu
		UINT right;	// �E�ʒu
	};
	// �t�H���g�̍�����62�A����48�s�N�Z��
	const int FONT_BORDER = 3;      // �E�Ɖ���1�s�N�Z�����̓����ȋ��E�{���Z�����E
	const int FONT_WIDTH = 48;		// �t�H���g��48�s�N�Z���̕�
	const int FONT_HEIGHT = 62;     // �t�H���g��62�s�N�Z���̍���
	const int GRID_WIDTH = FONT_WIDTH + FONT_BORDER;	// �����I�ȃt�H���g�̕�
	const int GRID_HEIGHT = FONT_HEIGHT + FONT_BORDER;	// �����I�ȃt�H���g�̍���
	const int COLUMNS = 16;         // �t�H���g�̉摜�̍s��
	const int ROWS = 14;            // �t�H���g�̉摜�̗�
	const int FRAMES = 1;           // �A�j���[�V�����̃t���[�����i1 = �A�j���[�V�����Ȃ��j
	const double ANIM_DELAY = 0.0;  // �A�j���[�V�����̃t���[���Ԃ̎���
	const int MAX_FONT_HEIGHT = 1000;	// �ő�̃t�H���g�̍���
	const int MIN_CHAR = 0x0020;    // �ŏ������R�[�h
	const int MAX_CHAR = 0x00FF;    // �ő啶���R�[�h
	const int PROPORTIONAL_SPACING = 5; // �v���|�[�V���i���t�H���g��1:1�X�P�[���̏ꍇ�̕����Ԃ̃X�y�[�X
	const int TAB_SIZE = 8;			// �^�u�̕�
	const char UNDERLINE = '_';		// ����
	const char SOLID = 0x7F;        // �R�[�h$7F�̕����Ɏg�p�����Œ�u���b�N
	const int BOLD_SIZE = 4;        // �����\���̂Ƃ��̃V�t�g����s�N�Z����
}

//==========================================================
// �X�v���C�g�e�L�X�g��`�悷��N���X�ł��B
class Text : public Image
{
private:
	TextureManager mFontTexture;		// ���ꂼ��̃t�H���g�̃e�N�X�`��
	LP_TEXTURE mTextureData;			// �ꎞ�ۑ��p�e�N�X�`���f�[�^
	char    *mFile;						// �e�N�X�`���t�@�C����
	Graphics *mGraphics;				// graphics�ւ̃|�C���^
	UINT mWidth, mHeight;				// 1�����̕��A����
	textNS::FontData mFontData[textNS::ROWS][textNS::COLUMNS]; // ���ꂼ��̕����̍��[�ƉE�[
	COLOR_ARGB mColor;					// �t�H���g�J���[(a,r,g,b)
	COLOR_ARGB mBackColor;				// �w�i�F(a,r,g,b)
	UINT  mFontHeight;					// �t�H���g�̍���
	UINT  mTabSize;						// �^�u�̕�
	UINT  mProportionalSpacing;			// �v���|�[�V���i���t�H���g�̕����Ԃ̃X�y�[�X
	bool mProportional;					// �v���|�[�V���i���t�H���g�̏ꍇ�Atrue
	bool mUnderline;					// ��������̏ꍇ�Atrue
	bool mBold;							// �����̏ꍇ�Atrue
	textNS::Alignment mAlign;			// �A���C�������g(center, left, etc)

public:
	// �f�t�H���g�R���X�g���N�^
	Text();

	// �f�X�g���N�^
	virtual ~Text();

	//=============================================================
	// �p�����������o�֐�
	//=============================================================

	// Text��������
	// �t�H���g�摜���̊e�����̍��[�ƉE�[�����
	// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
	// fontData�z��́A�e�����ƍ��[�ƉE�[���i�[
	virtual bool initialize(Graphics *g, const char *file);
	virtual void update(const float frameTime) {};
	virtual void onLostDevice();
	virtual void onResetDevice();

	//=============================================================
	// �V���������o�֐�
	//=============================================================

	// �\������ʒu�iXY���W�j���Z�b�g
	virtual void setXY(int x, int y);

	// �������X�AY�ɏo�́BspriteBegin()��spriteEnd()�̊ԂɌĂяo��
	virtual void print(const std::string &str);

	// �������X�AY�ɏo�́BspriteBegin()��spriteEnd()�̊ԂɌĂяo��
	virtual void print(const std::string &str, int x, int y);

	// �������X�AY�ɏo�́BspriteBegin()��spriteEnd()�̊ԂɌĂяo��
	virtual void print(const std::string &str, int x, int y, textNS::Alignment align);

	// ���݂̃t�H���g�T�C�Y�ł̕�����̕��ƍ����i�s�N�Z���P�ʁj���擾
	// ������͕\�����Ȃ�
	virtual void getWidthHeight(const std::string &str, UINT &width, UINT &height);

	// �t�H���g�J���[���Z�b�g
	virtual void setFontColor(COLOR_ARGB c) { mColor = c; }

	// �t�H���g�J���[��߂�
	virtual COLOR_ARGB getFontColor() { return mColor; }

	// �w�i�F���Z�b�g
	virtual void setBackColor(COLOR_ARGB bc) { mBackColor = bc; }

	// �w�i�F��߂�
	virtual COLOR_ARGB getBackColor() { return mBackColor; }

	// �t�H���g�̍������Z�b�g
	virtual void setFontHeight(UINT height)
	{
		if (height == 0 || height > textNS::MAX_FONT_HEIGHT)  // �s�\�ȍ����̏ꍇ
			return;
		mFontHeight = height;
		mSpriteData.scale = (float)height / (float)textNS::FONT_HEIGHT;
	}

	// �t�H���g�̍�����߂�
	virtual UINT getFontHeight() { return mFontHeight; }

	// �v���|�[�V���i���t�H���g���ǂ������Z�b�g
	virtual void setProportional(bool p) { mProportional = p; }

	// �v���|�[�V���i���t�H���g���ǂ�����߂�
	virtual bool getProportional() { return mProportional; }

	// �v���|�[�V���i���t�H���g�̕����Ԃ̃X�y�[�X���Z�b�g
	virtual void setProportionalSpacing(UINT s) { mProportionalSpacing = s; }

	// �v���|�[�V���i���t�H���g�̕����Ԃ̃X�y�[�X��߂�
	virtual UINT getProportionalSpacing() { return mProportionalSpacing; }

	// ������\�����邩�ǂ������Z�b�g
	virtual void setUnderline(bool u) { mUnderline = u; }

	// ������\�����邩�ǂ�����߂�
	virtual bool getUnderline() { return mUnderline; }

	// �����ɂ��邩�ǂ������Z�b�g
	virtual void setBold(bool b) { mBold = b; }

	// �����ɂ��邩�ǂ�����߂�
	virtual bool getBold() { return mBold; }

	// �^�u�̃T�C�Y���Z�b�g
	virtual void setTabSize(UINT size)
	{
		if (size == 0)
			return;
		mTabSize = size;
	}

	// �^�u�̃T�C�Y��Ԃ�
	virtual UINT getTabSize() { return mTabSize; }

	// ���݂̕�����Ɣz�u�̎w��ɍ��킹��spriteData.x�AspriteData.y��ݒ�
	// �f�t�H���g�̔z�u��LEFT
	virtual void doAlign(const std::string &str);

	// spriteDate�ɂ���ċL�q����镶���X�v���C�g��F�Ɠh��Ԃ����g���ĕ\��
	// �����ƕ�����\������
	virtual void drawChar(UCHAR ch);
};

#endif

