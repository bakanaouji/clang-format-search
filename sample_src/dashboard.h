//==========================================================
/// @file
/// @brief    �_�b�V���{�[�h�֘A�̃N���X
/// @author   �������V
///
/// @attention  �_�b�V���{�[�h�i�_�O���t�A�X�C�b�`�Ȃǁj��\���N���X�ł��B
///				�V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p���邱�Ƃ��ł��܂��B

//==========================================================
#ifndef _DASHBOARD_H            // ���̃t�@�C���������̉ӏ��ŃC���N���[�h�����ꍇ�ɁA 
#define _DASHBOARD_H            // ���d�ɒ�`����邱�Ƃ�h���܂��B
#define WIN32_LEAN_AND_MEAN
//==========================================================

#include <string>
#include <sstream>
#include "image.h"
#include "constants.h"
#include "textureManager.h"
#include "input.h"

//==========================================================

// �_�b�V���{�[�h�̒萔
// �Q�[�����ł̃X�e�[�^�X�ȊO�������ɋL�q
namespace dashboardNS
{
	const int	WIDTH = 48;				// �摜�̕��i�e�t���[���j
	const int	HEGIHT = 48;			// �摜�̍���
    const int   TEXTURE_COLS = 4;       // �e�N�X�`����4��
    const int   BAR_FRAME = 0;          // ���ꂼ��̃e�N�X�`���̃t���[���ԍ�
    const int   DIAL360_FRAME = 1;      // "
    const int   DIAL270_FRAME = 2;      // ...
    const int   DIAL180_FRAME = 3;
    const int   LIGHT_FRAME = 4;
    const int   POINTER_FRAME = 5;
    const int   SEGMENT_FRAME = 6;
    const int   DECIMAL_FRAME = 7;
    const int   SWITCH_OFF_FRAME = 8;
    const int   SWITCH_ON_FRAME = 9;
    const int   BUTTON_UP_FRAME = 10;
    const int   BUTTON_DOWN_FRAME = 11;
    const int   BAR_GRAPH_FRAME = 12;
    const int   SWITCH_WIDTH = 44;      // �g�O���X�C�b�`�̕��i�s�N�Z���P�ʁj
    const int   BAR_GRAPH_WIDTH = 8;    // �_�O���t�̕��{�󂫃X�y�[�X
    enum DialType{DIAL360, DIAL270, DIAL180};
}

// �Z�u���Z�O�����g��\���N���X�ł��B
// �V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p���邱�Ƃ��ł��܂��B
class SevenSegment : public Image
{
private:
    Image   mDecimal;	// �Z�u���Z�O�����g�̉摜
    UINT    mDigits;	// ����
    double  mNumber;	// �\�����鐔�l
public:
	// �R���X�g���N�^
    SevenSegment();
	// �Z�u���Z�O�����g�\���̏�����
	// �����F*graphics	Graphics�I�u�W�F�N�g�ւ̃|�C���^
	// �����F*textureM	TextureManager�I�u�W�F�N�g�ւ̃|�C���^
	// �����Fleft�Atop	��ʈʒu
	// �����Fscale		�{���i�Y�[���j
	// �����Fdigits		����
	// �����Fcolor		�����̐F
	// �߂�l�F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, UINT digits, const COLOR_ARGB color);
	// �Z�u���Z�O�����g�\���ɕ\�����鐔�l��ݒ�
	// �����Fvalue	�\�����鐔�l
    void set(const double value);
	// Update
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
    virtual void update(const float frameTime)    {}
	// �Z�u���Z�O�����g�̐����u0�v�`�u9�v�Ɓu-�v��\��
	// �����Fn	�\�����鐔�l
	// �����Fcolor	�\������F
    void drawDigit(const char n, const COLOR_ARGB color);
	// �����_��`��
	// �����Fcolor	�\������F
    void drawDecimal(const COLOR_ARGB color);
	// �Z�u���Z�O�����g�\����`��
	// number�ϐ��ɂ́A�\�����镂�������_���l���i�[����Ă���
	// �����Fcolor	�\������F�i�f�t�H���g�ł͔��j
	virtual void draw(const COLOR_ARGB color = graphicsNS::WHITE);
};

// �o�[��\���N���X�ł��B
// �V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p���邱�Ƃ��ł��܂��B
class Bar : public Image
{
public:
	// Bar��������
	// �����F*graphics	Graphics�I�u�W�F�N�g�ւ̃|�C���^
	// �����F*textureM	TextureManager�I�u�W�F�N�g�ւ̃|�C���^
	// �����Fleft�Atop	��ʈʒu
	// �����Fscale		�{���i�Y�[���j
	// �����Fcolor		�o�[�̐F
	// �߂�l�F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, const COLOR_ARGB color);
	// �o�[�̃T�C�Y��ݒ�
	// �����FpercentOn	�o�[�̃T�C�Y
    void set(float percentOn);
	// Update
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
    virtual void update(const float frameTime)    {}
};

// �~�O���t��\���N���X�ł��B
// �V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p���邱�Ƃ��ł��܂��B
class DialGauge : public Image
{
private:
    Image   mPointer;					// �摜
    dashboardNS::DialType mDialType;	// �~�Ղ̎��
public:
	// DialGauge��������
	// �����F*graphics	Graphics�I�u�W�F�N�g�ւ̃|�C���^
	// �����F*textureM	TextureManager�I�u�W�F�N�g�ւ̃|�C���^
	// �����Fleft�Atop	��ʈʒu
	// �����Fscale		�{���i�Y�[���j
	// �����Ftype		�~�Ղ̎��
	// �����FzeroAngle	�~�Ղ̃[���̈ʒu
	// �����FdialColor	�~�Ղ̐F
	// �����FpointColor	�����̐F
	// �߂�l�F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, const dashboardNS::DialType type, const float zeroAngle,
                    const COLOR_ARGB dialColor, const COLOR_ARGB pointerColor);
	// DialGauge�̎w�j��ݒ�
	// �����FpercentOn	�w�j�̒l
    void set(float percentOn);
	// Update
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
    virtual void update(const float frameTime)    {}
	// DialGauge�Ǝw�j��`��
	// �����Fcolor	�\������F�i�f�t�H���g�ł͔��j
    virtual void draw(const COLOR_ARGB color = graphicsNS::WHITE); // �t�B���^�[�Ƃ���color���g�p���ĕ`��
};

// ���C�g��\���N���X�ł��B
// �V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p���邱�Ƃ��ł��܂��B
class Light : public Image
{
private:
    float   mFlashTimer;	// ���C�g�_�ŗp�̃^�C�}�[
    float   mFlashDelay;	// ���C�g���_�ł���Ԋu
    COLOR_ARGB  mOnColor;	// �_�����̃��C�g�̐F
    COLOR_ARGB  mOffColor;	// �������̃��C�g�̐F
public:
	// �R���X�g���N�^
    Light();
	// Light��������
	// �����F*graphics	Graphics�I�u�W�F�N�g�ւ̃|�C���^
	// �����F*textureM	TextureManager�I�u�W�F�N�g�ւ̃|�C���^
	// �����Fleft�Atop	��ʈʒu
	// �����Fscale		�{���i�Y�[���j
	// ����flashRate	�_��/����/�_�ł̑���
	//		   �i<0�̏ꍇ�͓_���A=0�̏ꍇ�͏����A>0�̏ꍇ��1�b������̓_�ł̎��ԁj
	// �����FcolorOn	�_�����̃��C�g�̐F
	// �����FcolorOff	�������̃��C�g�̐F
	// �߂�l�F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                const float scale, const float flashRate, const COLOR_ARGB colorOn, const COLOR_ARGB colorOff);
	// flashRate��ݒ�F<0�̏ꍇ�͓_���A =0�̏ꍇ�͏����A>0�̏ꍇ�͓_�ł̎���
	// �����Frate	flashRate
	void set(const float rate);

	// Update
	// ���C�g���X�V�B
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
    virtual void update(const float frameTime);
};

// �g�O���X�C�b�`��\���N���X�ł��B
// �V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p���邱�Ƃ��ł��܂��B
class ToggleSwitch : public Image
{
private:
    Input   *mInput;			// ���̓V�X�e���ւ̃|�C���^
    HWND    mHwnd;				// �E�B���h�E�n���h��
    RECT    mSwitchRect;        // �}�E�X�N���b�N���󂯕t����͈�
    bool    mSwitchOn;          // �X�C�b�`�̏��
    bool    mMouseClick;        // �}�E�X�N���b�N�̏��
    public:
    // Toggle switch�R���X�g���N�^
    ToggleSwitch();
	// �؂�ւ��X�C�b�`�iToggleSwitch�j��������
	// �����F*graphics	Graphics�I�u�W�F�N�g�ւ̃|�C���^
	// �����F*textureM	TextureManager�I�u�W�F�N�g�ւ̃|�C���^
	// �����F*in		Input�I�u�W�F�N�g�ւ̃|�C���^
	// �����Fhwnd		�E�B���h�E�ւ̃n���h��
	// �����Fleft�Atop	��ʈʒu
	// �����Fscale		�{���i�Y�[���j
	// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
    bool initialize(Graphics *graphics, TextureManager *textureM, Input *in, const HWND hwnd,
                    const int left, const int top, const float scale);
	// Update
	// �X�C�b�`��ł̃}�E�X�N���b�N���`�F�b�N
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
    virtual void update(const float frameTime);
    // �X�C�b�`�̏�Ԃ��擾
	// �߂�l�F�X�C�b�`���I�����ǂ���
    bool getSwitchOn()   {return mSwitchOn;}
    // �X�C�b�`�̏�Ԃ�ݒ�
	// �����Fon	�X�C�b�`���I�����ǂ���
    void setSwitch(const bool on) {mSwitchOn = on;}
};

// �v�b�V���{�^����\���N���X�ł��B
// �V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p���邱�Ƃ��ł��܂��B
class PushButton : public Image
{
private:
	Input   *mInput;			// ���̓V�X�e���ւ̃|�C���^
	HWND    mHwnd;				// �E�B���h�E�n���h��
	RECT    mSwitchRect;        // �}�E�X�N���b�N���󂯕t����͈�
	bool    mSwitchOn;          // �X�C�b�`�̏��
	bool    mMouseClick;        // �}�E�X�N���b�N�̏��
    bool    mMomentary;         // ��莞�Ԃ��Ƃɐ؂�ւ���ꍇ��true�A�g�O���̏ꍇ��false
public:
    // Pushbutton switch�R���X�g���N�^
    PushButton();
	// �v�b�V���{�^���iPushButton�j��������
	// �����F*graphics	Graphics�I�u�W�F�N�g�ւ̃|�C���^
	// �����F*textureM	TextureManager�I�u�W�F�N�g�ւ̃|�C���^
	// �����F*in		Input�I�u�W�F�N�g�ւ̃|�C���^
	// �����Fhwnd		�E�B���h�E�ւ̃n���h��
	// �����Fleft�Atop	��ʈʒu
	// �����Fscale		�{���i�Y�[���j
	// �����Ftype		true�̏ꍇ�̓��[�����^���[�Afalse�̏ꍇ�̓I���^�l�[�g
	// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
    bool initialize(Graphics *graphics, TextureManager *textureM, Input *in, const HWND hwnd,
                    const int left, const int top, const float scale, const bool momentary);
	// Update
	// �v�b�V���{�^����ł̃}�E�X�N���b�N���`�F�b�N
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
    virtual void update(const float frameTime);
	// �X�C�b�`�̏�Ԃ��擾
	// �߂�l�F�X�C�b�`���I�����ǂ���
    bool getSwitchOn()  {return mSwitchOn;}
	// �X�C�b�`�̏�Ԃ�ݒ�
	// �����Fon	�X�C�b�`���I�����ǂ���
    void setSwitch(const bool on) {mSwitchOn = on;}
};


// �_�O���t��\���N���X�ł��B
// �V�X�e���O���t�B�b�N�X�Ƃ��Ďg�p���邱�Ƃ��ł��܂��B
class BarGraph : public Image
{
private:
    int mMaxBars;			// �o�[�̖{���̍ő吔
    int mBarsOn;			// ���݂̃o�[�̖{��
public:
    // BarGraph�R���X�g���N�^
    BarGraph();
	// BarGraph��������
	// �����F*graphics	Graphics�I�u�W�F�N�g�ւ̃|�C���^
	// �����F*textureM	TextureManager�I�u�W�F�N�g�ւ̃|�C���^
	// �����Fleft�Atop	��ʈʒu
	// �����Fscale		�{���i�Y�[���j
	// �����Fbars		���[�^�[���̃o�[�̖{��
	// �����Fcolor		�o�[�̐F
	// ���s��F���������ꍇ��true�A�G���[�̏ꍇ��false��߂�
    bool initialize(Graphics *graphics, TextureManager *textureM, const int left, const int top,
                    const float scale, const UINT bars, const COLOR_ARGB color);
	// barsOn�̒l���A�\������o�[�̖{���ɐݒ�
	// �p�����[�^p�͊����i0�`100�j
    void set(float percentOn);
	// Update
	// �ʏ�A�t���[�����Ƃ�1��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	// �����FframeTime�@1�t���[���Ōo�߂�������
    virtual void update(const float frameTime)    {}
	// BarGraph��`��
	// barsOn�́A�\������o�[�̖{�����i�[
	// �����Fcolor	�\������F�i�f�t�H���g�ł͔��j
    virtual void draw(const COLOR_ARGB color = graphicsNS::WHITE); // �t�B���^�[�Ƃ���color���g�p���ĕ`��
};

#endif

