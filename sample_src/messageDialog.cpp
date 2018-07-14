//==========================================================
/// @file
/// @brief    massageDialog.h�̎���
/// @author   �������V
///
/// @attention  ���[�U�[�ւ̃��b�Z�[�W�_�C�A���O��\���N���X�ł��B

#include "messageDialog.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
MessageDialog::MessageDialog()
{
    mInitialized = false;                // ����ɏ��������ꂽ�ꍇ�Atrue��ݒ�
    mGraphics = NULL;
    mVisible = false;                    // ��\���ɂ���
    mFontColor = messageDialogNS::FONT_COLOR;
    mBorderColor = messageDialogNS::BORDER_COLOR;
    mBackColor = messageDialogNS::BACK_COLOR;
    mButtonColor = messageDialogNS::BUTTON_COLOR;
    mButtonFontColor = messageDialogNS::BUTTON_FONT_COLOR;
    mX = messageDialogNS::X;             // �J�n�ʒu
    mY = messageDialogNS::Y;
    mHeight = messageDialogNS::HEIGHT;
    mWidth = messageDialogNS::WIDTH;
    mTextRect.bottom = messageDialogNS::Y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN;
    mTextRect.left = messageDialogNS::X + messageDialogNS::MARGIN;
    mTextRect.right = messageDialogNS::X + messageDialogNS::WIDTH - messageDialogNS::MARGIN;
    mTextRect.top = messageDialogNS::Y + messageDialogNS::MARGIN;
    mDialogVerts = NULL;
    mBorderVerts = NULL;
    mButtonVerts = NULL;
    mButton2Verts = NULL;
    mButtonType = 0;     // OK/Cancel
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
MessageDialog::~MessageDialog()
{
    onLostDevice();            // ���ׂẴO���t�B�b�N�A�C�e���ɂ���onLostDevice()���Ăяo��
}

//=============================================================================
// MessageDialog��������
//=============================================================================
bool MessageDialog::initialize(Graphics *g, Input *in, HWND h)
{
    try {
        mGraphics = g;                   // Graphics�I�u�W�F�N�g
        mInput = in;                     // Input�I�u�W�F�N�g
        mHwnd = h;

		// DirectX�t�H���g��������
        if(mDxFont.initialize(mGraphics, messageDialogNS::FONT_HEIGHT, false, 
                             false, messageDialogNS::FONT) == false)
            return false;               // ���s�̏ꍇ
        mDxFont.setFontColor(mFontColor);
    } catch(...) {
        return false;
    }

    mInitialized = true;
    return true;
}

//=============================================================================
// �_�C�A���O�̔w�i�ƃ{�^����`�悷�邽�߂ɒ��_�o�b�t�@������
//=============================================================================
void MessageDialog::prepareVerts()
{
    SAFE_RELEASE(mDialogVerts);
    SAFE_RELEASE(mBorderVerts);
    SAFE_RELEASE(mButtonVerts);
    SAFE_RELEASE(mButton2Verts);

	// �g�̍���
    mVtx[0].x = mX;
    mVtx[0].y = mY;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mBorderColor;

    // �g�̉E��
    mVtx[1].x = mX + mWidth;
    mVtx[1].y = mY;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mBorderColor;

	// �g�̉E��
    mVtx[2].x = mX + mWidth;
    mVtx[2].y = mY + mHeight;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mBorderColor;

	// �g�̍���
    mVtx[3].x = mX;
    mVtx[3].y = mY + mHeight;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mBorderColor;

    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mBorderVerts);

    // background�̍���
    mVtx[0].x = mX + messageDialogNS::BORDER;
    mVtx[0].y = mY + messageDialogNS::BORDER;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mBackColor;

    // background�̉E��
    mVtx[1].x = mX + mWidth - messageDialogNS::BORDER;
    mVtx[1].y = mY + messageDialogNS::BORDER;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mBackColor;

    // background�̉E��
    mVtx[2].x = mX + mWidth - messageDialogNS::BORDER;
    mVtx[2].y = mY + mHeight - messageDialogNS::BORDER;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mBackColor;

    // background�̍���
    mVtx[3].x = mX + messageDialogNS::BORDER;
    mVtx[3].y = mY + mHeight - messageDialogNS::BORDER;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mBackColor;

    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mDialogVerts);

    // button�̍���
    mVtx[0].x = mX + mWidth/2.0f - messageDialogNS::BUTTON_WIDTH/2.0f;
    mVtx[0].y = mY + mHeight - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mButtonColor;

    // button�̉E��
    mVtx[1].x = mX + mWidth/2.0f + messageDialogNS::BUTTON_WIDTH/2.0f;
    mVtx[1].y = mVtx[0].y;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mButtonColor;

    // button�̉E��
    mVtx[2].x =  mVtx[1].x;
    mVtx[2].y = mVtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mButtonColor;

    // button�̍���
    mVtx[3].x = mVtx[0].x;
    mVtx[3].y = mVtx[2].y;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mButtonColor;

    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mButtonVerts);

    // button��Rect��ݒ�
    mButtonRect.left   = (long)mVtx[0].x;
    mButtonRect.right  = (long)mVtx[1].x;
    mButtonRect.top    = (long)mVtx[0].y;
    mButtonRect.bottom = (long)mVtx[2].y;

    // button2�̍���
    mVtx[0].x = mX + mWidth - messageDialogNS::BUTTON_WIDTH*1.2f;
    mVtx[0].y = mY + mHeight - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
    mVtx[0].z = 0.0f;
    mVtx[0].rhw = 1.0f;
    mVtx[0].color = mButtonColor;
    // button2�̉E��
    mVtx[1].x = mVtx[0].x + messageDialogNS::BUTTON_WIDTH;
    mVtx[1].y = mVtx[0].y;
    mVtx[1].z = 0.0f;
    mVtx[1].rhw = 1.0f;
    mVtx[1].color = mButtonColor;
    // button2�̉E��
    mVtx[2].x =  mVtx[1].x;
    mVtx[2].y = mVtx[0].y + messageDialogNS::BUTTON_HEIGHT;
    mVtx[2].z = 0.0f;
    mVtx[2].rhw = 1.0f;
    mVtx[2].color = mButtonColor;
    // button2�̍���
    mVtx[3].x = mVtx[0].x;
    mVtx[3].y = mVtx[2].y;
    mVtx[3].z = 0.0f;
    mVtx[3].rhw = 1.0f;
    mVtx[3].color = mButtonColor;
    mGraphics->createVertexBuffer(mVtx, sizeof mVtx, mButton2Verts);

	// button2��Rect��ݒ�
    mButton2Rect.left   = (long)mVtx[0].x;
    mButton2Rect.right  = (long)mVtx[1].x;
    mButton2Rect.top    = (long)mVtx[0].y;
    mButton2Rect.bottom = (long)mVtx[2].y;
}

//=============================================================================
// MessageDialog��`��
//=============================================================================
const void MessageDialog::draw()
{
    if (!mVisible || mGraphics == NULL || !mInitialized)
        return;

    mGraphics->drawQuad(mBorderVerts);        // �g��`��
    mGraphics->drawQuad(mDialogVerts);        // �w�i��`��
    mGraphics->drawQuad(mButtonVerts);        // �{�^����`��
    mGraphics->drawQuad(mButton2Verts);       // �{�^��2��`��

    mGraphics->spriteBegin();				  // �X�v���C�g�̕`����J�n

    if(mText.size() == 0)
        return;
	// �e�L�X�g��MassegeDialog�ɕ\��
    mDxFont.setFontColor(mFontColor);
    mDxFont.print(mText,mTextRect,DT_CENTER|DT_WORDBREAK);

	// �e�L�X�g���{�^���ɕ\��
    mDxFont.setFontColor(mButtonFontColor);
    mDxFont.print(messageDialogNS::BUTTON1_TEXT[mButtonType],mButtonRect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    mDxFont.print(messageDialogNS::BUTTON2_TEXT[mButtonType],mButton2Rect,
                 DT_SINGLELINE|DT_CENTER|DT_VCENTER);

    mGraphics->spriteEnd();                  // �X�v���C�g�̕`����J�n
}

//=============================================================================
// DIALOG_CLOSE_KEY��OK�{�^���̃N���b�N���`�F�b�N
//=============================================================================
void MessageDialog::update()
{
    if (!mInitialized || !mVisible)
        return;
    if (mInput->wasKeyPressed(messageDialogNS::DIALOG_CLOSE_KEY))
    {
        mVisible = false;
        mButtonClicked = 1;              // �{�^��1���N���b�N���ꂽ
        return;
    }

    if (mGraphics->getFullscreen() == false) // �E�B���h�E�\���̏ꍇ
    {
		// �E�B���h�E���T�C�Y�ύX����Ă���ꍇ���l�����āA��ʂ̔䗦���v�Z
        RECT clientRect;
        GetClientRect(mHwnd, &clientRect);
        mScreenRatioX = (float)GAME_WIDTH / clientRect.right;
        mScreenRatioY = (float)GAME_HEIGHT / clientRect.bottom;
    }

    if (mInput->getMouseLButton())       // �}�E�X�̍��{�^���̏ꍇ
    {
		// �}�E�X�N���b�N���{�^��1�iOK�j�̓����Ŕ��������ꍇ
        if (mInput->getMouseX()*mScreenRatioX >= mButtonRect.left &&
            mInput->getMouseX()*mScreenRatioX <= mButtonRect.right &&
            mInput->getMouseY()*mScreenRatioY >= mButtonRect.top &&
            mInput->getMouseY()*mScreenRatioY <= mButtonRect.bottom)
        {
            mVisible = false;            // ���b�Z�[�W�_�C�A���O���\��
            mButtonClicked = 1;          // �{�^��1���N���b�N���ꂽ
            return;
        }

		// �}�E�X�N���b�N�{�^��2�iCancel�j�̓����Ŕ��������ꍇ
        if (mInput->getMouseX()*mScreenRatioX >= mButton2Rect.left &&
            mInput->getMouseX()*mScreenRatioX <= mButton2Rect.right &&
            mInput->getMouseY()*mScreenRatioY >= mButton2Rect.top &&
            mInput->getMouseY()*mScreenRatioY <= mButton2Rect.bottom)
        {
            mVisible = false;            // ���b�Z�[�W�_�C�A���O���\��
            mButtonClicked = 2;          // �{�^��2���N���b�N���ꂽ
        }
    }
}

//=============================================================================
// �e�L�X�g�������ݒ�A�_�C�A���O�̉��[���e�L�X�g�����܂�T�C�Y�ɕύX�A
// visible = true��ݒ�
//=============================================================================
void MessageDialog::print(const std::string &str)         
{
	// ����������Ă��Ȃ��A�܂��͊��Ɏg�p���̏ꍇ
    if (!mInitialized || mVisible)
        return;
    mText = str + "\n\n\n\n";        // �{�^���p�ɋ�Ԃ��󂯂�

	// textRect���_�C�A���O�̃e�L�X�g�̈�Ƃ��Đݒ�
    mTextRect.left   = (long)(mX + messageDialogNS::MARGIN);
    mTextRect.right  = (long)(mX + messageDialogNS::WIDTH - messageDialogNS::MARGIN);
    mTextRect.top    = (long)(mY + messageDialogNS::MARGIN);
    mTextRect.bottom = (long)(mY + messageDialogNS::HEIGHT - messageDialogNS::MARGIN);

	// textRect.bottom���e�L�X�g�ɕK�v�ȍ����ɐݒ�
	// DT_CALCERCT�I�v�V�����̏ꍇ�A�e�L�X�g�͏o�͂���Ȃ�
    mDxFont.print(mText,mTextRect,DT_CENTER|DT_WORDBREAK|DT_CALCRECT);
    mHeight = mTextRect.bottom - (int)mY + messageDialogNS::BORDER + messageDialogNS::MARGIN;

    prepareVerts();                 // ���_�o�b�t�@������
    mButtonClicked = 0;             // buttonClicked�N���A
    mVisible = true;
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����������Ƃ��ɌĂяo��
//=============================================================================
void MessageDialog::onLostDevice()
{
    if (!mInitialized)
        return;
    mDxFont.onLostDevice();
    SAFE_RELEASE(mDialogVerts);
    SAFE_RELEASE(mBorderVerts);
    SAFE_RELEASE(mButtonVerts);
    SAFE_RELEASE(mButton2Verts);
}

//=============================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo��
//=============================================================================
void MessageDialog::onResetDevice()
{
    if (!mInitialized)
        return;
    prepareVerts();
    mDxFont.onResetDevice();
}

