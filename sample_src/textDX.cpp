//==========================================================
/// @file
/// @brief    textDX.h�̎���
/// @author   �������V
///
/// @attention  DXFONT�x�[�X�̃e�L�X�g��`�悷��N���X�ł��B

#include "textDX.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
TextDX::TextDX()
{
    mColor = SETCOLOR_ARGB(255,255,255,255); // �f�t�H���g�͔�����

	// �t�H���g�̈ʒu���Z�b�g
    mFontRect.top = 0;
    mFontRect.left = 0;
    mFontRect.right = GAME_WIDTH;
    mFontRect.bottom = GAME_HEIGHT;
    mDxFont = NULL;
    mAngle  = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
TextDX::~TextDX()
{
    SAFE_RELEASE(mDxFont);
}

//=============================================================================
// DirectX�t�H���g���쐬
//=============================================================================
bool TextDX::initialize(Graphics *g, int height, bool bold, bool italic, 
                        const std::string &fontName)
{
    mGraphics = g;                   // �O���t�B�b�N�V�X�e��

    UINT weight = FW_NORMAL;
    if(bold)
        weight = FW_BOLD;

	// DirectX�t�H���g���쐬
    if(FAILED(D3DXCreateFont(mGraphics->get3Ddevice(), height, 0, weight, 1, italic,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(),
        &mDxFont))) return false;

    // �ϊ��s���������
    D3DXMatrixTransformation2D(&mMatrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);

    return true;
}

//=============================================================================
// �e�L�X�g��x�Ay�ɏo��
// ���s���ɂ�0���A�������ɂ̓e�L�X�g�̍�����߂�
// ���s�O�FspriteBegin()
// ���s��FspriteEnd()
//=============================================================================
int TextDX::print(const std::string &str, int x, int y)
{
    if(mDxFont == NULL)
        return 0;
	// �t�H���g�ʒu��ݒ�
    mFontRect.top = y;
    mFontRect.left = x;

	// ��]���S
    D3DXVECTOR2 rCenter=D3DXVECTOR2((float)x,(float)y);
    // �e�L�X�g�����̊p�x�ŉ�]����s�������
    D3DXMatrixTransformation2D(&mMatrix, NULL, 0.0f, NULL, &rCenter, mAngle, NULL);
    // �s����X�v���C�g�ɓK�p
    mGraphics->getSprite()->SetTransform(&mMatrix);
    return mDxFont->DrawTextA(mGraphics->getSprite(), str.c_str(), -1, &mFontRect, DT_LEFT, mColor);
}

//=============================================================================
// DirectX�e�L�X�g�������g���āARECT���Ƀe�L�X�g���o��
// ���s���ɂ�0���A�������ɂ̓e�L�X�g�̍�����߂�
// ���s�O�FspriteBegin()
// ���s��FspriteEnd()
//=============================================================================
int TextDX::print(const std::string &str, RECT &rect, UINT format)
{
    if(mDxFont == NULL)
        return 0;

	// �e�L�X�g����]�����Ȃ��s�������
    D3DXMatrixTransformation2D(&mMatrix, NULL, 0.0f, NULL, NULL, NULL, NULL);
	// �s����X�v���C�g�ɓK�p
    mGraphics->getSprite()->SetTransform(&mMatrix);
    return mDxFont->DrawTextA(mGraphics->getSprite(), str.c_str(), -1, &rect, format, mColor);
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo�����
//=============================================================================
void TextDX::onLostDevice()
{
    if(mDxFont == NULL)
        return;
    mDxFont->OnLostDevice();
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
//=============================================================================
void TextDX::onResetDevice()
{
    if(mDxFont == NULL)
        return;
    mDxFont->OnResetDevice();
}
