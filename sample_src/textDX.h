//==========================================================
/// @file
/// @brief    TextDX�N���X
/// @author   �������V
///
/// @attention  DXFONT�x�[�X�̃e�L�X�g��`�悷��N���X�ł��B

//==========================================================
#ifndef _TEXTDX_H               // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA
#define _TEXTDX_H               // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include "constants.h"
#include "graphics.h"

//==========================================================
// DXFONT�x�[�X�̃e�L�X�g��`�悷��N���X�ł��B
class TextDX
{
private:
    Graphics    *mGraphics;		// graphics�ւ̃|�C���^
    COLOR_ARGB  mColor;         // �t�H���g�J���[(a,r,g,b)
    LP_DXFONT   mDxFont;		// DXFONT
    RECT        mFontRect;      // �e�L�X�g�̎l�p�`
    D3DXMATRIX  mMatrix;		// �e�L�X�g�̉�]�s��
    float       mAngle;         // �e�L�X�g�̉�]�p�x

public:
	// �R���X�g���N�^
    TextDX();

	// �f�X�g���N�^
    virtual ~TextDX();

	// DirectX�t�H���g���쐬
    // ���s�O�F*g = Graphics�I�u�W�F�N�g�ւ̃|�C���^
    //		   height = ����
    //         bold = true/false
    //         italic = true/false
    //         &fontName = �g�p����t�H���g��
    virtual bool initialize(Graphics *g, int height, bool bold, bool italic, const std::string &fontName);

	// �e�L�X�g��x�Ay�ɏo��
	// ���s���ɂ�0���A�������ɂ̓e�L�X�g�̍�����߂�
	// ���s�O�FspriteBegin()
	// ���s��FspriteEnd()
    virtual int print(const std::string &str, int x, int y);

	// DirectX�e�L�X�g�������g���āARECT���Ƀe�L�X�g���o��
	// ���s���ɂ�0���A�������ɂ̓e�L�X�g�̍�����߂�
	// ���s�O�FspriteBegin()
	// ���s��FspriteEnd()
    virtual int print(const std::string &str, RECT &rect, UINT format);

	// ��]�p��߂��i�x�j
    virtual float getDegrees()      {return mAngle*(180.0f/(float)PI);}

    // ��]�p��߂��i���W�A���j
    virtual float getRadians()      {return mAngle;}

	// �t�H���g�J���[��߂�
    virtual COLOR_ARGB getFontColor() {return mColor;}

	// ��]�p���Z�b�g�i�x�j
    virtual void setDegrees(float deg)  {mAngle = deg*((float)PI/180.0f);}

	// ��]�p���Z�b�g�i���W�A���j
    virtual void setRadians(float rad)  {mAngle = rad;}

    // �t�H���g�J���[���Z�b�g
	virtual void setFontColor(COLOR_ARGB c) {mColor = c;}

	// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
    virtual void onLostDevice();

	// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
    virtual void onResetDevice();
};

#endif

